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
#include <fstream>
#include <string>

#include "raster_geometry.hpp"
#include "winraster_renderer.hpp"
#include "projected_point.hpp"

WinRasterRenderer::WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd)
   : iProjectionWidth_(0), iProjectionHeight_(0), pProjectionBuffer_(nullptr), iLastSizePB_{ 0 },
   numBytesInRow_{ 0 }, pBitmapBuffer_(nullptr), iLastSizeBB_{ 0 }, rectLast_{ 0 }, mt_(6),
   iLeftMargin_{ 0 }, iTopMargin_{ 0 }, iRightMargin_{ 0 }, iBottomMargin_{ 0 },
   iBitmapWidth_{ 0 }, iBitmapHeight_{ 0 }
{
	hwnd_ = hwnd;
	hdc_ = GetDC( hwnd );

	pRasterGeom_ = std::make_unique<RasterGeometry>(colorFileName);
   colorRefBackground_ = crBgrnd;

   projector_.init(pRasterGeom_.get());
   composer_.init(GetBValue(colorRefBackground_), GetGValue(colorRefBackground_), GetRValue(colorRefBackground_));
   driverMT_.init(std::thread::hardware_concurrency());
}

WinRasterRenderer::~WinRasterRenderer(void)
{
   ReleaseDC(hwnd_, hdc_);
   driverMT_.join();
   mt_.save("winraster_timing.txt");
}

void WinRasterRenderer::backgroundJob(void)
{
   mt_.start();
	pRasterGeom_->nextFrame();
   mt_.check(0);

   RECT rectBoundingBox{ 0 };
   rectBoundingBox.left = static_cast<LONG>(pRasterGeom_->getMinTransformedX());
   rectBoundingBox.top = static_cast<LONG>(pRasterGeom_->getMinTransformedY());
   rectBoundingBox.right = static_cast<LONG>(pRasterGeom_->getMaxTransformedX() + 1);
   rectBoundingBox.bottom = static_cast<LONG>(pRasterGeom_->getMaxTransformedY() + 1);
   mt_.check(1);

   initProjectionBuffer_();
   mt_.check(2);

   projector_.setupProjection(iProjectionHeight_, iProjectionWidth_, pProjectionBuffer_.get());
   driverMT_.drive(&projector_);
   mt_.check(3);

   initBitmapBuffer_(rectBoundingBox);
   composer_.setupProjection(iProjectionHeight_, iProjectionWidth_, pProjectionBuffer_.get());
   composer_.setupBitmap(iLeftMargin_, iBottomMargin_, numBytesInRow_, pBitmapBuffer_.get());
   driverMT_.drive(&composer_);
   mt_.check(4);
      
   drawBitmap_(rectBoundingBox);
   mt_.check(5);

   // Save the detail of the current bounding box.
   // We'll use it to erase the content in the next cycle.
   rectLast_ = rectBoundingBox;
}

void WinRasterRenderer::initProjectionBuffer_()
{
   iProjectionWidth_ = pRasterGeom_->getMaxTransformedX() -
		pRasterGeom_->getMinTransformedX() + 1;
   iProjectionHeight_ = pRasterGeom_->getMaxTransformedY() -
		pRasterGeom_->getMinTransformedY() + 1;

   size_t	numberOfProjectedPixels = iProjectionWidth_ * iProjectionHeight_;
   if (numberOfProjectedPixels > iLastSizePB_)
   {
      iLastSizePB_ = numberOfProjectedPixels;
      pProjectionBuffer_ = std::make_unique<ProjectedPoint[]>(iLastSizePB_);
   }

// Initialise the space for projecting the pixels
	ProjectedPoint blackPoint{ 0 };
   ProjectedPoint* pProjData = pProjectionBuffer_.get();
	for(size_t pixIndex = 0; pixIndex < numberOfProjectedPixels; pixIndex++ )
      pProjData[ pixIndex ] = blackPoint;
}

void WinRasterRenderer::initBitmapBuffer_(RECT& rectBoundingBox)
{
   iLeftMargin_ = 0;
   if (rectLast_.left < rectBoundingBox.left)
      iLeftMargin_ = size_t(rectBoundingBox.left) - size_t(rectLast_.left);

   iRightMargin_ = 0;
   if (rectLast_.right > rectBoundingBox.right)
      iRightMargin_ = size_t(rectLast_.right) - size_t(rectBoundingBox.right);

   iTopMargin_ = 0;
   if (rectLast_.top < rectBoundingBox.top)
      iTopMargin_ = size_t(rectBoundingBox.top) - size_t(rectLast_.top);

   iBottomMargin_ = 0;
   if (rectLast_.bottom > rectBoundingBox.bottom)
      iBottomMargin_ = size_t(rectLast_.bottom) - size_t(rectBoundingBox.bottom);

   iBitmapWidth_ = iProjectionWidth_ + iLeftMargin_ + iRightMargin_;
   iBitmapHeight_ = iProjectionHeight_ + iTopMargin_ + iBottomMargin_;

   numBytesInRow_ = 3 * iBitmapWidth_;
   if (numBytesInRow_ % 4 != 0)
      numBytesInRow_ += 4 - numBytesInRow_ % 4;

   // Give it 4 more bytes to accomodate unconditional padding at the end
   // of each row to keep the algorithm simple.
   size_t iNewBufferSize = numBytesInRow_ * (iBitmapHeight_) + 4;
   if (iNewBufferSize > iLastSizeBB_)
   {
      iLastSizeBB_ = iNewBufferSize;
      pBitmapBuffer_ = std::make_unique<unsigned char[]>(iLastSizeBB_);
   }
   else
      std::memset(pBitmapBuffer_.get(), 0, iLastSizeBB_);
}

void WinRasterRenderer::drawBitmap_(RECT& rectBoundingBox)
{
   BITMAPINFOHEADER infoHeader{ 0 };
   infoHeader.biSize = sizeof(BITMAPINFOHEADER);
   infoHeader.biWidth = static_cast<LONG>(iBitmapWidth_);
   infoHeader.biHeight = static_cast<LONG>(iBitmapHeight_);
   infoHeader.biPlanes = 1;
   infoHeader.biBitCount = 24;
   infoHeader.biCompression = BI_RGB;
   infoHeader.biSizeImage = 0;
   infoHeader.biXPelsPerMeter = 1000;
   infoHeader.biYPelsPerMeter = 1000;
   infoHeader.biClrUsed = 0;
   infoHeader.biClrImportant = 0;

   SetDIBitsToDevice(
      hdc_,
      int(rectBoundingBox.left) - int(iLeftMargin_),	// X destination
      int(rectBoundingBox.top) - int(iTopMargin_),	   // Y destination
      static_cast<DWORD>(iBitmapWidth_),
      static_cast<DWORD>(iBitmapHeight_),
      0, 0, 0, static_cast<UINT>(iBitmapHeight_),
      pBitmapBuffer_.get(),
      (BITMAPINFO*)(&infoHeader),
      DIB_RGB_COLORS
   );
}

void WinRasterRenderer::setSize(unsigned winWidth, unsigned winHeight)
{
	pRasterGeom_->setSize(winWidth, winHeight);
}
