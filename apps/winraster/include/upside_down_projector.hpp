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

#ifndef upside_down_projector_hpp
#define upside_down_projector_hpp

#include <thread>
#include <atomic>

#include <mutex>
#include <condition_variable>

class RasterGeometry;
struct ProjectedPoint;

// The following atomwrapper structure has been copy/pasted from:
// https://stackoverflow.com/questions/13193484/how-to-declare-a-vector-of-atomic-in-c
template <typename T>
struct atomwrapper
{
   std::atomic<T> _a;

   atomwrapper()
      :_a()
   {}

   atomwrapper(const std::atomic<T>& a)
      :_a(a.load())
   {}

   atomwrapper(const atomwrapper& other)
      :_a(other._a.load())
   {}

   atomwrapper& operator=(const atomwrapper& other)
   {
      _a.store(other._a.load());
      return *this;
   }
};

class UpsideDownProjector {
public:
   UpsideDownProjector() noexcept;
   void init(RasterGeometry* pRasterGeom);
   void operator()(size_t inxThread, size_t iNumThreads, size_t iRasterHeight) noexcept;
   void project(size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer);
   void join();

private:
   void project_(size_t inxBeginRow, size_t inxEndRow) noexcept;

   size_t iProjectionWidth_;
   RasterGeometry* pRasterGeom_;
   ProjectedPoint* pProjectedData_;

   std::vector<std::thread> helperThreads_;
   std::vector<atomwrapper<size_t>> helperThreadControls_;

   // Helper thread synchronisation objects.
   std::mutex mutexHT_;
   std::condition_variable cvHT_;

   // Main thread synchronisation objects.
   std::mutex mutexMT_;
   std::condition_variable cvMT_;
};

#endif // upside_down_projector_hpp
