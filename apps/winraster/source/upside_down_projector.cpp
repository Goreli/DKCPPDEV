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
   : iRasterWidth_{ 0 }, iProjectionWidth_{ 0 }, pRasterGeom_{ nullptr }, pImageData_{ nullptr }
{
}
void UpsideDownProjector::init(size_t rasterWidth, size_t projectionWidth, RasterGeometry* pRasterGeom, ProjectedPixel* pImageData) noexcept
{
   iRasterWidth_ = rasterWidth;
   iProjectionWidth_ = projectionWidth;
   pRasterGeom_ = pRasterGeom;
   pImageData_ = pImageData;
}
void UpsideDownProjector::defaultFunction(size_t inxBeginRow, size_t inxEndRow) noexcept
{
   for (size_t inxRow = inxBeginRow; inxRow < inxEndRow; inxRow++)
      for (size_t inxCol = 0; inxCol < iRasterWidth_; inxCol++)
      {
         // Get coordinates of the pixel relative to the projection
         // Inverse the projectionRow to project everything upside down
         size_t iProjectionRow = pRasterGeom_->getTransformedY(inxCol, inxRow);
         size_t iProjectionColumn = pRasterGeom_->getTransformedX(inxCol, inxRow);
         // Copy colors from raster to projection
         ProjectedPixel* pPixel = pImageData_ - iProjectionRow * iProjectionWidth_ + iProjectionColumn;
         pPixel->r += pRasterGeom_->getRed(inxCol, inxRow);
         pPixel->g += pRasterGeom_->getGreen(inxCol, inxRow);
         pPixel->b += pRasterGeom_->getBlue(inxCol, inxRow);
         pPixel->counter++;
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

   defaultFunction(inxBeginRow, inxEndRow);
}

void UpsideDownProjector::runThreads(size_t numThreads, size_t iRasterHeight)
{
   std::vector<std::thread> threads;
   for (size_t inxThread = 0; inxThread < numThreads; inxThread++)
      threads.push_back(std::thread(std::ref(*this), inxThread, numThreads, iRasterHeight));
   for (std::thread& t : threads)
      t.join();
}
