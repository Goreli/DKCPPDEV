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

#include "winraster_renderer.hpp"
#include "raster_geometry.hpp"

struct WinRasterRenderer::ProjectedPixel_
{
	unsigned long b;
	unsigned long g;
	unsigned long r;
	unsigned long counter;
};

WinRasterRenderer::WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd)
	: pBitmap_(nullptr), bitmapWidth_(0), bitmapHeight_(0), rectLast_{ 0 }
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
}

void WinRasterRenderer::eraseLastRect()
{
   FillRect(hdc_, &rectLast_, hBrushBG_);
}

void WinRasterRenderer::backgroundJob(void)
{
	pRasterGeom_->nextFrame();
	initBitmapData_();
	projectPixelsUpsideDown_();
	projection2ActualBitmap_();
        
   eraseLastRect();

   rectLast_.left = pRasterGeom_->getMinTransformedX();
   rectLast_.top = pRasterGeom_->getMinTransformedY();
   rectLast_.right = pRasterGeom_->getMaxTransformedX() + 1;
   rectLast_.bottom = pRasterGeom_->getMaxTransformedY() + 1;

   // Draw the bitmap
   SetDIBitsToDevice(
      hdc_,
      rectLast_.left,	// X destination
      rectLast_.top,	   // Y destination
      bitmapWidth_,
      bitmapHeight_,
      0, 0, 0, bitmapHeight_,
      getImageData_(),
      (BITMAPINFO*)(pBitmap_.get()),
      DIB_RGB_COLORS
   );
}

void WinRasterRenderer::initBitmapData_()
{
	bitmapWidth_ = pRasterGeom_->getMaxTransformedX() -
		pRasterGeom_->getMinTransformedX() + 1;
	bitmapHeight_ = pRasterGeom_->getMaxTransformedY() -
		pRasterGeom_->getMinTransformedY() + 1;
	pBitmap_ = std::make_unique<unsigned char[]>(sizeof(BITMAPINFOHEADER) +
		bitmapWidth_ * bitmapHeight_ * sizeof(ProjectedPixel_));

	BITMAPINFOHEADER* pInfoHeader = (BITMAPINFOHEADER*)(pBitmap_.get());
	pInfoHeader->biSize = sizeof( BITMAPINFOHEADER );
	pInfoHeader->biWidth = bitmapWidth_;
	pInfoHeader->biHeight = bitmapHeight_;
	pInfoHeader->biPlanes = 1;
	pInfoHeader->biBitCount = 24;
	pInfoHeader->biCompression = BI_RGB;
	pInfoHeader->biSizeImage = 0;
	pInfoHeader->biXPelsPerMeter = 1000;
	pInfoHeader->biYPelsPerMeter = 1000;
	pInfoHeader->biClrUsed = 0;
	pInfoHeader->biClrImportant = 0;

// Initialise the space for projecting the pixels
	int	numberOfPixels = bitmapWidth_ * bitmapHeight_;
	ProjectedPixel_ blackPixel{ 0 };
	ProjectedPixel_* pImageData = getImageData_();
	for(int pixIndex = 0; pixIndex < numberOfPixels; pixIndex++ )
		pImageData[ pixIndex ] = blackPixel;
}

void WinRasterRenderer::projectPixelsUpsideDown_()
{
int rasterRow;
int rasterColumn;
int projectionRow;
int pry, prmy;
int projectionColumn;
int rasterRows = pRasterGeom_->rasterHeight();
int rasterColumns = pRasterGeom_->rasterWidth();
ProjectedPixel_*pPixel;
ProjectedPixel_* pImageData = getImageData_();

	for( rasterRow = 0; rasterRow < rasterRows; rasterRow++ )
		for( rasterColumn = 0; rasterColumn < rasterColumns; rasterColumn++ )
		{
			// Get coordinates of the pixel relative to the projection
			// Inverse the projectionRow to project everything upside down
			pry = pRasterGeom_->getTransformedY(rasterColumn, rasterRow);
			prmy = pRasterGeom_->getMinTransformedY();
			projectionRow = bitmapHeight_ - 1 - (pry - prmy);
			projectionColumn = pRasterGeom_->getTransformedX( rasterColumn, rasterRow )
					- pRasterGeom_->getMinTransformedX();
			// Copy colors from the raster to the projection
			pPixel = pImageData + projectionRow * bitmapWidth_ + projectionColumn;
			pPixel->r += pRasterGeom_->getRed( rasterColumn, rasterRow );
			pPixel->g += pRasterGeom_->getGreen( rasterColumn, rasterRow );
			pPixel->b += pRasterGeom_->getBlue( rasterColumn, rasterRow );
			pPixel->counter++;
		}
}

void WinRasterRenderer::projection2ActualBitmap_()
{
unsigned char *bColor;
ProjectedPixel_* pImageData = getImageData_();
ProjectedPixel_* pPixel = pImageData;
int	numOfBytesInRow = 3 * bitmapWidth_;

	if( numOfBytesInRow % 4  !=  0 )
		numOfBytesInRow += 4 - numOfBytesInRow % 4;

	for( int row = 0; row < bitmapHeight_; row++ )
	{
		bColor = (unsigned char *)pImageData + row * numOfBytesInRow;
		for( int col = 0; col < bitmapWidth_; col++ )
		{
		    if( pPixel->counter != 0 )
		    {
			*bColor++ = static_cast<unsigned char>(pPixel->b / pPixel->counter);
			*bColor++ = static_cast<unsigned char>(pPixel->g / pPixel->counter);
			*bColor++ = static_cast<unsigned char>(pPixel->r / pPixel->counter);
		    }
		    else
		    {
			*bColor++ = GetBValue(colorRefBackground_);
			*bColor++ = GetGValue(colorRefBackground_);
			*bColor++ = GetRValue(colorRefBackground_);
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

WinRasterRenderer::ProjectedPixel_* WinRasterRenderer::getImageData_()
{
	return (ProjectedPixel_*)(pBitmap_.get() + sizeof(BITMAPINFOHEADER));
}
