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
#include "multi_threaded_driver.hpp"

ParallelProcessorBase::~ParallelProcessorBase()
{
}

MultiThreadedDriver::MultiThreadedDriver()
   : pParProc_{ nullptr }
{
}
MultiThreadedDriver::~MultiThreadedDriver()
{
   if (helperThreads_.size() > 0)
      join();
}

void MultiThreadedDriver::init(size_t iNumThreads)
{
   for (size_t inxThread = 0; inxThread < iNumThreads; inxThread++)
   {
      helperThreadControls_.push_back(0);
      helperThreads_.push_back(std::thread(std::ref(*this), inxThread));
   }
}

void MultiThreadedDriver::join()
{
   {
      std::unique_lock<std::mutex> ulockHT(mutexHT_);
      // Requst the thread functions to exit to make it possible
      // for the threads to join the main thread.
      for (auto& iTC : helperThreadControls_)
         iTC = 1;
   }

   cvHT_.notify_all();

   for (std::thread& t : helperThreads_)
      t.join();

   helperThreadControls_.clear();
   helperThreads_.clear();
}

void MultiThreadedDriver::drive(ParallelProcessorBase* pParProc)
{
   if (helperThreads_.size() == 0)
      pParProc->operator()(0, pParProc->size());
   else
   {
      pParProc_ = pParProc;

      {
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         for (auto& iTC : helperThreadControls_)
            iTC = 2;
      }

      cvHT_.notify_all();

      // Check if the job has been done.
      std::unique_lock<std::mutex> ulockMT(mutexMT_);
      cvMT_.wait(ulockMT,
         [&] {
            for (auto tc : helperThreadControls_)
               if (tc != 0)
                  return false;
            return true;
         });
   }
}

void MultiThreadedDriver::operator()(size_t inxThread)
{
   while (true)
   {
      {
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         cvHT_.wait(ulockHT, [&] {return helperThreadControls_[inxThread] != 0; });
      }

      size_t iChunkSize = pParProc_->size() / helperThreads_.size();
      size_t inxBeginRow = inxThread * iChunkSize;
      size_t inxEndRow = inxBeginRow + iChunkSize;
      // If this is the last chunk/thread then its endRow value may need to be adjusted.
      if (inxThread + 1 == helperThreads_.size())
         inxEndRow = pParProc_->size();

      switch (helperThreadControls_[inxThread])
      {
      case 1:
         return;
      case 2:
         pParProc_->operator()(inxBeginRow, inxEndRow);
         // Notify the main thread.
         {
            std::unique_lock<std::mutex> ulockMT(mutexMT_);
            // Let the main thread know that the job has been done.
            helperThreadControls_[inxThread] = 0;
         }
         cvMT_.notify_one();
         break;
      default:
         break;
      }
   }
}
