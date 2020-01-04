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

#ifndef dk_multi_threaded_driver
#define dk_multi_threaded_driver

#include <thread>
#include <mutex>
#include <vector>

namespace dk {

class ParallelProcessorBase;

class MultiThreadedDriver
{
public:
   MultiThreadedDriver();
   virtual ~MultiThreadedDriver();

   void init(size_t iNumThreads);
   void join();
   void drive(ParallelProcessorBase* pParProc);
   void operator()(size_t inxThread);

private:
   std::vector<size_t> helperThreadControls_;
   std::vector<std::thread> helperThreads_;

   // Helper thread synchronisation objects.
   std::mutex mutexHT_;
   std::condition_variable cvHT_;

   // Main thread synchronisation objects.
   std::mutex mutexMT_;
   std::condition_variable cvMT_;

   ParallelProcessorBase* pParProc_;
};

}  // namespace dk {

#endif // dk_multi_threaded_driver
