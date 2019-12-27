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

void UpsideDownProjector::init(RasterGeometry* pRasterGeom, size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer) noexcept
{
   pRasterGeom_ = pRasterGeom;
   iProjectionWidth_ = iProjectionWidth;

   size_t projectionRowPreCalc = pRasterGeom_->getMinTransformedY()
      + iProjectionHeight - 1;

   pProjectedData_ = pProjectionBuffer
      + projectionRowPreCalc * iProjectionWidth_
      - pRasterGeom_->getMinTransformedX();
}
void UpsideDownProjector::project()
{
   size_t numThreads{ std::thread::hardware_concurrency() };
   // If there are no concurrent threads available or the size of the image
   // is relatively small then do the calculation in the main thread.
   size_t iRasterHeight = pRasterGeom_->rasterHeight();
   if (numThreads == 0 || numThreads > iRasterHeight)
      project_(0, iRasterHeight);
   else
   {
      std::vector<std::thread> threads;
      size_t iRasterHeight = pRasterGeom_->rasterHeight();
      for (size_t inxThread = 0; inxThread < numThreads; inxThread++)
         threads.push_back(std::thread(std::ref(*this), inxThread, numThreads, iRasterHeight));
      for (std::thread& t : threads)
         t.join();
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

   project_(inxBeginRow, inxEndRow);
}

