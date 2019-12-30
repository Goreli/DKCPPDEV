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

#include "upside_down_projector.hpp"
#include "winraster_renderer.hpp"
#include "raster_geometry.hpp"

UpsideDownProjector::UpsideDownProjector()
   : pRasterGeom_{ nullptr }, iProjectionWidth_{ 0 }, pProjectedData_{ nullptr }
{}
UpsideDownProjector::~UpsideDownProjector()
{}
void UpsideDownProjector::init(RasterGeometry* pRasterGeom)
{
   pRasterGeom_ = pRasterGeom;
}
void UpsideDownProjector::setupProjection(size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer)
{
   iProjectionWidth_ = iProjectionWidth;

   size_t projectionRowPreCalc = pRasterGeom_->getMinTransformedY()
      + iProjectionHeight - 1;

   pProjectedData_ = pProjectionBuffer
      + projectionRowPreCalc * iProjectionWidth_
      - pRasterGeom_->getMinTransformedX();
}
void UpsideDownProjector::operator()(size_t inxBegin, size_t inxEnd)
{
   size_t iRasterWidth = pRasterGeom_->rasterWidth();
   for (size_t inxRow = inxBegin; inxRow < inxEnd; inxRow++)
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
size_t UpsideDownProjector::size()
{
   return pRasterGeom_->rasterHeight();
}
