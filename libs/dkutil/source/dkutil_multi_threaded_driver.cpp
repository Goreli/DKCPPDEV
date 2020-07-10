/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#include <functional>
#include "dkutil_multi_threaded_driver.hpp"
#include "dkutil_parallel_processor_base.hpp"

using namespace dk;

ParallelProcessorBase::~ParallelProcessorBase()
{
}
size_t ParallelProcessorBase::partition_size_limit() {
   return 0;
}

MultiThreadedDriver::MultiThreadedDriver()
   : pParProc_{ nullptr }, inxNextPartStart_{ 0 }
{
}
MultiThreadedDriver::~MultiThreadedDriver()
{
   join();
}
void MultiThreadedDriver::init(size_t iNumThreads)
{
   join();

   for (size_t inxThread = 0; inxThread < iNumThreads; inxThread++)
   {
      // Init the state of the thread to 0.
      helperThreadStates_.push_back(0);
      // Now start the thread and record it in helperThreads_.
      helperThreads_.push_back(std::thread(std::ref(*this), inxThread));
      // This has effectively started execution of 'operator()(inxThread)'
   }
}
void MultiThreadedDriver::operator()(size_t inxThread)
{
   while (true)
   {
      {
         // The thread is waiting here after either:
         // - The init() function has just created the thread and put
         //   it in the wait state;
         // - Either execSinglePartition_() or execMultiplePartitions_() has
         //   has just completed the job and put the thread back in the wait
         //   state.
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         cvHT_.wait(ulockHT, [&] {return helperThreadStates_[inxThread] != 0; });
      }

      switch (helperThreadStates_[inxThread])
      {
      case 1:
         return;
      case 2:
         execSinglePartition_(inxThread);
         break;
      case 3:
         execMultiplePartitions_(inxThread);
         break;
      default:
         break;
      }
   }
}
void MultiThreadedDriver::drive(ParallelProcessorBase* pParProc)
{
   if (helperThreads_.size() == 0)
      pParProc->operator()(0, pParProc->size());
   else
   {
      pParProc_ = pParProc;

      {
         // Identify the execution mode and prepare the respective state code.
         // If task partitions are allowed to be of any size then we will use
         // a state code 2. In this mode we will generate and execute exactly
         // the same number of partitions as the number of threads initialised
         // with the init() function.
         // Otherwise, if the partitions have a specified size, then we will
         // use a state code 3. In this mode each thread, as it completes the
         // previous partition, will be picking up the next available partition
         // to execute.
         size_t iExecMode{ 2 };
         if (pParProc_->partition_size_limit() > 0)
         {
            iExecMode = 3;
            inxNextPartStart_ = 0;
         }

         // Lock out helperThreadStates_ and set all elements to the state
         // code prepared above. This will convey a message to the threads
         // indicating the expected execution mode.
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         for (auto& iTC : helperThreadStates_)
            iTC = iExecMode;
         // The message has been conveyed so let's unlock helperThreadStates_
         // by getting ulockHT out of scope.
      }

      // Now when the message about the expected execution mode has been
      // conveyed to all threads let's trigger the actual execution by
      // requesting them to examine the wait condition they are stuck at.
      cvHT_.notify_all();

      // Check if the job has been done.
      std::unique_lock<std::mutex> ulockMT(mutexMT_);
      cvMT_.wait(ulockMT,
         [&] {
            for (auto tc : helperThreadStates_)
               if (tc != 0)
                  return false;
            return true;
         });
   }
}
void MultiThreadedDriver::execSinglePartition_(size_t inxThread)
{
   size_t inxPartStart{ 0 };
   size_t inxPartEnd{ 0 };
   // In this mode we will break up
   // the task into as many partitions as the number of threads allocated
   // in helperThreads_. So each thread will process exactly one partition
   // regardless of the total number of threads.
   // What we want is to preclude outstandingly large partitions in the set.
   // In other words we prefer evenly sized partitions. Examples follow.
   //
   // Example 1. Task size = 10 symbolic units; Number of threads = 4.
   // We prefer the following pattern of partition size values:
   // 3, 3, 2, 2 rather than 4, 2, 2, 2.
   //
   // Example 2. Task size = 11 symbolic units; Number of threads = 4.
   // We prefer the following pattern of partition size values:
   // 3, 3, 3, 2 rather than 5, 2, 2, 2.

   size_t iPartitionSize = pParProc_->size() / helperThreads_.size();
   size_t iRemainder = pParProc_->size() % helperThreads_.size();
   if (inxThread < iRemainder)
   {
      iPartitionSize++;
      inxPartStart = inxThread * iPartitionSize;
      inxPartEnd = inxPartStart + iPartitionSize;
   }
   else
   {
      inxPartStart = inxThread * iPartitionSize + iRemainder;
      inxPartEnd = inxPartStart + iPartitionSize;
   }

   // Execute the task partition assigned to this thread.
   pParProc_->operator()(inxPartStart, inxPartEnd);

   notifyMainThread_(inxThread);
}
void MultiThreadedDriver::execMultiplePartitions_(size_t inxThread)
{
   size_t inxPartStart{ 0 };
   size_t inxPartEnd{ 0 };
   bool bNothingToDo{ false };
   size_t iTaskSize = pParProc_->size();

   while (true)
   {
      {  // Lock out the other threads and check if there are
         // any outstanding partitions to be executed.
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         if (inxNextPartStart_ < iTaskSize)
         {
            // Set up the current partition boundary variables
            // and advance the next partition pointer.
            inxPartStart = inxNextPartStart_;
            inxNextPartStart_ += pParProc_->partition_size_limit();
            if (inxNextPartStart_ > iTaskSize)
               inxNextPartStart_ = iTaskSize;
            inxPartEnd = inxNextPartStart_;
         }
         else
            bNothingToDo = true;
      }  // The other threads are now unlocked.

      if (bNothingToDo)
         break;

      // Execute the task partition picked by this thread.
      pParProc_->operator()(inxPartStart, inxPartEnd);
   }

   notifyMainThread_(inxThread);
}
void MultiThreadedDriver::notifyMainThread_(size_t inxThread)
{
   {
      std::unique_lock<std::mutex> ulockMT(mutexMT_);
      // Let the main thread know that the job has been done.
      helperThreadStates_[inxThread] = 0;
   }
   cvMT_.notify_one();
}
void MultiThreadedDriver::join()
{
   if (helperThreads_.size() == 0)
      return;

   {
      std::unique_lock<std::mutex> ulockHT(mutexHT_);
      // Requst the thread functions to exit to make it possible
      // for the threads to join the main thread.
      for (auto& iTC : helperThreadStates_)
         iTC = 1;
   }

   cvHT_.notify_all();

   for (std::thread& t : helperThreads_)
      t.join();

   helperThreadStates_.clear();
   helperThreads_.clear();
}
