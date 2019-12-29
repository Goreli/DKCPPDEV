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
   : iNumRows_{0}, iProjectionWidth_{ 0 }, pRasterGeom_{ nullptr }, pProjectedData_{ nullptr }
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
      std::atomic<size_t> a_i(0);
      for (size_t inxThread = 0; inxThread < numThreads; inxThread++)
      {
         helperThreadControls_.push_back(a_i);
         helperThreads_.push_back(std::thread(std::ref(*this), inxThread, numThreads));
      }
   }
}

void UpsideDownProjector::join()
{
   {
      std::unique_lock<std::mutex> ulockHT(mutexHT_);
      // Requst the thread functions to exit to make it possible
      // for the threads to join the main thread.
      std::atomic<size_t> a_i(1);
      for (auto& iTC : helperThreadControls_)
         iTC = a_i;
   }

   cvHT_.notify_all();

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
      iNumRows_ = pRasterGeom_->rasterHeight();
      runThreads_(2);
   }
}

void UpsideDownProjector::populateBitmap(size_t iProjectionHeight, COLORREF colorRefBackground, size_t iBottomMargin,
   size_t numBytesInRow, size_t iLeftMargin, ProjectedPoint* pProjectionBuffer, unsigned char* pBitmapBuffer)
{
   colorRefBackground_ = colorRefBackground;
   iBottomMargin_ = iBottomMargin;
   numBytesInRow_ = numBytesInRow;
   iLeftMargin_ = iLeftMargin;

   pProjectionBuffer_ = pProjectionBuffer;
   pBitmapBuffer_ = pBitmapBuffer;

   if (helperThreads_.size() == 0)
      populateBitmap_(0, iProjectionHeight);
   else
   {
      iNumRows_ = iProjectionHeight;
      runThreads_(3);
   }
}
void UpsideDownProjector::runThreads_(size_t iTask)
{
   std::atomic<size_t> a_i(iTask);
   {
      std::unique_lock<std::mutex> ulockHT(mutexHT_);
      for (auto& iTC : helperThreadControls_)
         iTC = a_i;
   }

   cvHT_.notify_all();

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

void UpsideDownProjector::populateBitmap_(size_t inxBeginRow, size_t inxEndRow) noexcept
{
   for (size_t row = inxBeginRow; row < inxEndRow; row++)
   {
      ProjectedPoint* pProjPoint = pProjectionBuffer_ + row * iProjectionWidth_;
      unsigned char* pPixel = pBitmapBuffer_ + (row + iBottomMargin_) * numBytesInRow_ + iLeftMargin_ * 3;

      // These two variables are meant to help getting rid of the ugly
      // black mesh periodically occuring in the middle of the image and
      // supposidly caused by the rounding error.
      bool bThereWasNonBlank{ false };
      bool bHereIsAnotherBlank{ false };

      for (int col = 0; col < iProjectionWidth_; col++)
      {
         if (pProjPoint->counter == 0)
         {
            if (bThereWasNonBlank)
               bHereIsAnotherBlank = true;

            *pPixel++ = GetBValue(colorRefBackground_);
            *pPixel++ = GetGValue(colorRefBackground_);
            *pPixel++ = GetRValue(colorRefBackground_);
         }
         else
         {
            *pPixel++ = static_cast<unsigned char>(pProjPoint->b / pProjPoint->counter);
            *pPixel++ = static_cast<unsigned char>(pProjPoint->g / pProjPoint->counter);
            *pPixel++ = static_cast<unsigned char>(pProjPoint->r / pProjPoint->counter);

            // Check if there was a rounding gap and interpolate its colors if there was one.
            if (bHereIsAnotherBlank)
            {
               // Non-blank pixel after a blank one even though there was
               // already a non-blank pixel before the last blank one...
               // Let's fill the gap with interpolated color.
               unsigned char* pBlankPixel = pPixel - 6;

               // Interpolate blue.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;
               // Interpolate green.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;
               // Interpolate red.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;

               bHereIsAnotherBlank = false;
            }
            bThereWasNonBlank = true;
         }
         pProjPoint++;
      }
      // Possibly we have a padding at the end of each row.
      // Fill it with zeros. This is safe because elements
      // pointed to by pPixel are smaller than those pointed
      // to by pProjPoint. Therefore pPixel has remained well
      // below the upper boundary of allocated space.
      *pPixel = '\0';
      *(pPixel + 1) = '\0';
      *(pPixel + 2) = '\0';
   }
}

void UpsideDownProjector::operator()(size_t inxThread, size_t iNumThreads) noexcept
{
   while (true) 
   {
      {
         std::unique_lock<std::mutex> ulockHT(mutexHT_);
         cvHT_.wait(ulockHT, [&] {return helperThreadControls_[inxThread]._a != 0; });
      }

      size_t iChunkHeight = iNumRows_ / iNumThreads;
      size_t inxBeginRow = inxThread * iChunkHeight;
      size_t inxEndRow = inxBeginRow + iChunkHeight;
      // If this is the last chunk/thread then its endRow value may need to be adjusted.
      if (inxThread + 1 == iNumThreads)
         inxEndRow = iNumRows_;

      switch (helperThreadControls_[inxThread]._a)
      {
      case 1:
         return;
      case 2:
         project_(inxBeginRow, inxEndRow);
         NotifyMainThread_(inxThread);
         break;
      case 3:
         populateBitmap_(inxBeginRow, inxEndRow);
         NotifyMainThread_(inxThread);
         break;
      default:
         break;
      }
   }
}

void UpsideDownProjector::NotifyMainThread_(size_t inxThread)
{
   std::atomic<size_t> a_i(0);
   {
      std::unique_lock<std::mutex> ulockMT(mutexMT_);
      // Let the main thread know that the job has been done.
      helperThreadControls_[inxThread] = a_i;
   }
   cvMT_.notify_one();
}
