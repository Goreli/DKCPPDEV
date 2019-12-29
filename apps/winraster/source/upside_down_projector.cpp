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

#include <thread>
#include <vector>
#include "upside_down_projector.hpp"
#include "winraster_renderer.hpp"
#include "raster_geometry.hpp"

UpsideDownProjector::UpsideDownProjector() noexcept
   : iProjectionWidth_{ 0 }, pRasterGeom_{ nullptr }, pProjectedData_{ nullptr }
{
}

void UpsideDownProjector::init(RasterGeometry* pRasterGeom)
{
   pRasterGeom_ = pRasterGeom;

   size_t numThreads{ std::thread::hardware_concurrency() };
   // If there are no concurrent threads available or the size of the image
   // is relatively small then do the calculation in the main thread.
   size_t iRasterHeight = pRasterGeom_->rasterHeight();
   if (numThreads == 0 || numThreads > iRasterHeight)
      return;
   else
   {
      //threadControls_.resize(numThreads);
      //for (auto& iTC : threadControls_)
         //iTC = 0;

      std::atomic<size_t> a_i(0);
      for (size_t inxThread = 0; inxThread < numThreads; inxThread++)
      {
         helperThreadControls_.push_back(a_i);
         helperThreads_.push_back(std::thread(std::ref(*this), inxThread, numThreads, iRasterHeight));
      }
   }
}

void UpsideDownProjector::join()
{
   {
      std::unique_lock<std::mutex> ulockHT(mutexHT_);

      std::atomic<size_t> a_i(1);
      for (auto& iTC : helperThreadControls_)
         iTC = a_i;

      cvHT_.notify_all();
   }

   for (std::thread& t : helperThreads_)
      t.join();
}


void UpsideDownProjector::project(size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer)
{
   iProjectionWidth_ = iProjectionWidth;

   size_t projectionRowPreCalc = pRasterGeom_->getMinTransformedY()
      + iProjectionHeight - 1;

   pProjectedData_ = pProjectionBuffer
      + projectionRowPreCalc * iProjectionWidth_
      - pRasterGeom_->getMinTransformedX();

   if (helperThreads_.size() == 0)
      project_(0, pRasterGeom_->rasterHeight());
   else
   {
      {
         std::unique_lock<std::mutex> ulockHT(mutexHT_);

         std::atomic<size_t> a_i(2);

         for (auto& iTC : helperThreadControls_)
            iTC = a_i;

         //for (auto& iTC : helperThreadControls_)
            //while(iTC._a);
         cvHT_.notify_all();
      }


      // Check if the job has been done.
      std::unique_lock<std::mutex> ulockMT(mutexMT_);
      cvMT_.wait(ulockMT, 
         [&] {
            for (auto& tc : helperThreadControls_)
               if (tc._a != 0)
                  return false;
            return true;
         });
   }
}

void UpsideDownProjector::project_(size_t inxBeginRow, size_t inxEndRow) noexcept
{
   size_t iRasterWidth = pRasterGeom_->rasterWidth();
   for (size_t inxRow = inxBeginRow; inxRow < inxEndRow; inxRow++)
      for (size_t inxCol = 0; inxCol < iRasterWidth; inxCol++)
      {
         // Get coordinates of the pixel relative to the projection
         // Inverse the projectionRow to project everything upside down
         size_t iProjectionRow = pRasterGeom_->getTransformedY(inxCol, inxRow);
         size_t iProjectionColumn = pRasterGeom_->getTransformedX(inxCol, inxRow);
         // Copy colors from raster to projection
         ProjectedPoint* pPoint = pProjectedData_ - iProjectionRow * iProjectionWidth_ + iProjectionColumn;
         pPoint->r += pRasterGeom_->getRed(inxCol, inxRow);
         pPoint->g += pRasterGeom_->getGreen(inxCol, inxRow);
         pPoint->b += pRasterGeom_->getBlue(inxCol, inxRow);
         pPoint->counter++;
      }
}
void UpsideDownProjector::operator()(size_t inxThread, size_t iNumThreads, size_t iRasterHeight) noexcept
{
   size_t iChunkHeight = iRasterHeight / iNumThreads;
   size_t inxBeginRow = inxThread * iChunkHeight;
   size_t inxEndRow = inxBeginRow + iChunkHeight;
   // If this is the last chunk/thread then its endRow value may need to be adjusted.
   if (inxThread + 1 == iNumThreads)
      inxEndRow = iRasterHeight;

   std::atomic<size_t> a_i(0);
   while (true) 
   {
      {
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         cvHT_.wait(ulockHT, [&] {return helperThreadControls_[inxThread]._a != 0; });
      }

      switch (helperThreadControls_[inxThread]._a)
      {
      //case 0:
         //break;
      case 1:
         return;
      case 2:
         project_(inxBeginRow, inxEndRow);
         //threadControls_[inxThread]._a.store(0);
         // Let the main thread know that the job has been done.
         std::unique_lock<std::mutex> ulockMT(mutexMT_);
         helperThreadControls_[inxThread] = a_i;
         cvMT_.notify_one();
         break;
      }
      //std::this_thread::yield();  
   }
}

