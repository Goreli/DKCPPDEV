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

#include "winraster_renderer.hpp"
#include "raster_geometry.hpp"

WinRasterRenderer::WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd)
	: pBitmap_(nullptr), bitmapWidth_(0), bitmapHeight_(0), rectLast_{ 0 }, mt_(6)
{
	hwnd_ = hwnd;
	hdc_ = GetDC( hwnd );

	pRasterGeom_ = std::make_unique<RasterGeometry>(colorFileName);
	colorRefBackground_ = crBgrnd;
   hBrushBG_ = CreateSolidBrush(colorRefBackground_);
}

WinRasterRenderer::~WinRasterRenderer(void)
{
   DeleteObject(hBrushBG_);
   ReleaseDC(hwnd_, hdc_);

   mt_.save("winraster_timing.txt");
}

void WinRasterRenderer::eraseLastRect()
{
   FillRect(hdc_, &rectLast_, hBrushBG_);
}

/*
void WinRasterRenderer::drawImage_(RECT& rectBoundingBox)
{
   FillRect(hdc_, &rectLast_, hBrushBG_);
   // Draw the bitmap
   SetDIBitsToDevice(
      hdc_,
      rectBoundingBox.left,	// X destination
      rectBoundingBox.top,	   // Y destination
      static_cast<DWORD>(bitmapWidth_),
      static_cast<DWORD>(bitmapHeight_),
      0, 0, 0, static_cast<UINT>(bitmapHeight_),
      getImageData_(),
      (BITMAPINFO*)(pBitmap_.get()),
      DIB_RGB_COLORS
   );
}
*/
void WinRasterRenderer::drawImage_(RECT& rectBoundingBox)
{
   HDC hdcMem = CreateCompatibleDC(hdc_);

   FillRect(hdcMem, &rectLast_, hBrushBG_);
   // Draw the bitmap
   SetDIBitsToDevice(
      hdcMem,
      rectBoundingBox.left,	// X destination
      rectBoundingBox.top,	   // Y destination
      static_cast<DWORD>(bitmapWidth_),
      static_cast<DWORD>(bitmapHeight_),
      0, 0, 0, static_cast<UINT>(bitmapHeight_),
      getImageData_(),
      (BITMAPINFO*)(pBitmap_.get()),
      DIB_RGB_COLORS
   );

   BitBlt(
      hdc_, // handle to destination DC
      0,  // x-coord of destination upper-left corner
      0,  // y-coord of destination upper-left corner
      bitmapWidth_,  // width of destination rectangle
      bitmapHeight_, // height of destination rectangle
      hdcMem,  // handle to source DC
      0,   // x-coordinate of source upper-left corner
      0,   // y-coordinate of source upper-left corner
      SRCCOPY  // raster operation code
   );

   DeleteDC(hdcMem);
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

   initBitmapData_();
   mt_.check(2);

	projectPixelsUpsideDown_();
   mt_.check(3);

	projection2ActualBitmap_();
   mt_.check(4);
      
   drawImage_(rectBoundingBox);
   mt_.check(5);

   // Save the detail of the current bounding box.
   // We'll use it to erase the content in the next cycle.
   rectLast_ = rectBoundingBox;
}

void WinRasterRenderer::initBitmapData_()
{
	bitmapWidth_ = pRasterGeom_->getMaxTransformedX() -
		pRasterGeom_->getMinTransformedX() + 1;
	bitmapHeight_ = pRasterGeom_->getMaxTransformedY() -
		pRasterGeom_->getMinTransformedY() + 1;
	pBitmap_ = std::make_unique<unsigned char[]>(sizeof(BITMAPINFOHEADER) +
		bitmapWidth_ * bitmapHeight_ * sizeof(ProjectedPixel));

	BITMAPINFOHEADER* pInfoHeader = (BITMAPINFOHEADER*)(pBitmap_.get());
	pInfoHeader->biSize = sizeof( BITMAPINFOHEADER );
	pInfoHeader->biWidth = static_cast<LONG>(bitmapWidth_);
	pInfoHeader->biHeight = static_cast<LONG>(bitmapHeight_);
	pInfoHeader->biPlanes = 1;
	pInfoHeader->biBitCount = 24;
	pInfoHeader->biCompression = BI_RGB;
	pInfoHeader->biSizeImage = 0;
	pInfoHeader->biXPelsPerMeter = 1000;
	pInfoHeader->biYPelsPerMeter = 1000;
	pInfoHeader->biClrUsed = 0;
	pInfoHeader->biClrImportant = 0;

// Initialise the space for projecting the pixels
	size_t	numberOfPixels = bitmapWidth_ * bitmapHeight_;
	ProjectedPixel blackPixel{ 0 };
	ProjectedPixel* pImageData = getImageData_();
	for(size_t pixIndex = 0; pixIndex < numberOfPixels; pixIndex++ )
		pImageData[ pixIndex ] = blackPixel;
}

void WinRasterRenderer::projectPixelsUpsideDown_()
{
size_t projectionRowPreCalc = pRasterGeom_->getMinTransformedY()
                            + bitmapHeight_ - 1;
ProjectedPixel* pImageData = getImageData_()
                           + projectionRowPreCalc * bitmapWidth_ 
                           - pRasterGeom_->getMinTransformedX();

   udp_.init(pRasterGeom_->rasterWidth(), bitmapWidth_, pRasterGeom_.get(), pImageData);
   size_t numThreads{ std::thread::hardware_concurrency() };
   // If there are no concurrent threads available or the size of the image
   // is relatively small then do the calculation in the main thread.
   size_t rasterRows = pRasterGeom_->rasterHeight();
   if (numThreads == 0 || numThreads > size_t(rasterRows))
      udp_.defaultFunction(0, rasterRows);
   else
      udp_.runThreads(numThreads, rasterRows);
}

void WinRasterRenderer::projection2ActualBitmap_()
{
unsigned char *bColor;
ProjectedPixel* pImageData = getImageData_();
ProjectedPixel* pPixel = pImageData;
size_t	numOfBytesInRow = 3 * bitmapWidth_;

	if( numOfBytesInRow % 4  !=  0 )
		numOfBytesInRow += 4 - numOfBytesInRow % 4;

	for( size_t row = 0; row < bitmapHeight_; row++ )
	{
      // These two variables are meant to help getting rid of the ugly
      // black mesh periodically occuring in the middle of the image and
      // supposidly caused by the rounding error.
      bool bThereWasNonEmpty{ false };
      bool bHereIsAnotherEmpty{ false };

		bColor = (unsigned char *)pImageData + row * numOfBytesInRow;
		for( int col = 0; col < bitmapWidth_; col++ )
		{
		    if( pPixel->counter == 0 )
		    {
             if (bThereWasNonEmpty)
                bHereIsAnotherEmpty = true;

             *bColor++ = GetBValue(colorRefBackground_);
             *bColor++ = GetGValue(colorRefBackground_);
             *bColor++ = GetRValue(colorRefBackground_);
		    }
		    else
		    {
             *bColor++ = static_cast<unsigned char>(pPixel->b / pPixel->counter);
             *bColor++ = static_cast<unsigned char>(pPixel->g / pPixel->counter);
             *bColor++ = static_cast<unsigned char>(pPixel->r / pPixel->counter);

             // Check if there was a rounding gap and interpolate its colors if there was one.
             if (bHereIsAnotherEmpty)
             {
                // Non-empty cell after an empty one even though there was
                // already a non-empty cell before the last empty one...
                // Let's fill the gap with interpolated color.
                unsigned char* pEmptyCell = bColor - 6;

                // Interpolate blue.
                *pEmptyCell = pEmptyCell[-3] / 2 + pEmptyCell[3] / 2;
                pEmptyCell++;
                // Interpolate green.
                *pEmptyCell = pEmptyCell[-3] / 2 + pEmptyCell[3] / 2;
                pEmptyCell++;
                // Interpolate red.
                *pEmptyCell = pEmptyCell[-3] / 2 + pEmptyCell[3] / 2;
                pEmptyCell++;
                
                bHereIsAnotherEmpty = false;
             }
             bThereWasNonEmpty = true;
          }
		    pPixel++;
		}
		// Possibly we have a padding at the end of each row.
		// Fill it with zeros. This is safe because elements
		// pointed to by bColor are smaller than those pointed
		// to by pPixel. Therefore bColor has remained well
		// below the upper boundary of allocated space.
		*bColor = '\0';
		*(bColor+1) = '\0';
		*(bColor+2) = '\0';
	}
}

void WinRasterRenderer::setSize(unsigned winWidth, unsigned winHeight)
{
	pRasterGeom_->setSize(winWidth, winHeight);
}

ProjectedPixel* WinRasterRenderer::getImageData_()
{
	return (ProjectedPixel*)(pBitmap_.get() + sizeof(BITMAPINFOHEADER));
}
