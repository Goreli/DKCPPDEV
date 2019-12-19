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

struct ProjectedPixel
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

   std::ofstream ofs;
   ofs.open("winraster_timing.txt", std::ofstream::out | std::ofstream::app);

   ofs << '\t' << "Duration 1: " << duration1_.count() / iFrameCounter_ << std::endl;
   ofs << '\t' << "Duration 2: " << duration2_.count() / iFrameCounter_ << std::endl;
   ofs << '\t' << "Duration 3: " << duration3_.count() / iFrameCounter_ << std::endl;
   ofs << '\t' << "Duration 4: " << duration4_.count() / iFrameCounter_ << std::endl;
   ofs << '\t' << "Duration 5: " << duration5_.count() / iFrameCounter_ << std::endl;
   ofs << '\t' << "Duration 6: " << duration6_.count() / iFrameCounter_ << std::endl;
   ofs << std::endl;
}

void WinRasterRenderer::eraseLastRect()
{
   FillRect(hdc_, &rectLast_, hBrushBG_);
}

void WinRasterRenderer::backgroundJob(void)
{

   auto timeStamp1 = std::chrono::high_resolution_clock::now();

	pRasterGeom_->nextFrame();

   auto timeStamp2 = std::chrono::high_resolution_clock::now();

   RECT rectBoundingBox{ 0 };
   rectBoundingBox.left = static_cast<LONG>(pRasterGeom_->getMinTransformedX());
   rectBoundingBox.top = static_cast<LONG>(pRasterGeom_->getMinTransformedY());
   rectBoundingBox.right = static_cast<LONG>(pRasterGeom_->getMaxTransformedX() + 1);
   rectBoundingBox.bottom = static_cast<LONG>(pRasterGeom_->getMaxTransformedY() + 1);
 
   auto timeStamp3 = std::chrono::high_resolution_clock::now();

   initBitmapData_();

   auto timeStamp4 = std::chrono::high_resolution_clock::now();

	projectPixelsUpsideDown_();

   auto timeStamp5 = std::chrono::high_resolution_clock::now();

	projection2ActualBitmap_();

   auto timeStamp6 = std::chrono::high_resolution_clock::now();

   eraseLastRect();
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

   auto timeStamp7 = std::chrono::high_resolution_clock::now();

   rectLast_ = rectBoundingBox;

   duration1_ += timeStamp2 - timeStamp1;
   duration2_ += timeStamp3 - timeStamp2;
   duration3_ += timeStamp4 - timeStamp3;
   duration4_ += timeStamp5 - timeStamp4;
   duration5_ += timeStamp6 - timeStamp5;
   duration6_ += timeStamp7 - timeStamp6;
   iFrameCounter_++;
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

// Create a functor to use in a multithreading arrangement.
class UpsideDownProjector {
public:
   UpsideDownProjector();
   void init(size_t iCols, size_t bitmapWidth, RasterGeometry* pRasterGeom, ProjectedPixel* pImageData);
   void operator()(size_t inxFirstRow, size_t iRows) noexcept;
private:
   size_t iCols_;
   size_t bitmapWidth_;
   RasterGeometry* pRasterGeom_;
   ProjectedPixel* pImageData_;
};
UpsideDownProjector::UpsideDownProjector()
   : iCols_{ 0 }, bitmapWidth_{ 0 }, pRasterGeom_{ nullptr }, pImageData_{nullptr}
{
}
void UpsideDownProjector::init(size_t iCols, size_t bitmapWidth, RasterGeometry* pRasterGeom, ProjectedPixel* pImageData)
{
   iCols_ = iCols;
   bitmapWidth_ = bitmapWidth;
   pRasterGeom_ = pRasterGeom;
   pImageData_ = pImageData;
}

void UpsideDownProjector::operator()(size_t inxFirstRow, size_t inxEndRow) noexcept
{
   for (size_t inxRow = inxFirstRow; inxRow < inxEndRow; inxRow++)
      for (size_t inxCol = 0; inxCol < iCols_; inxCol++)
      {
         // Get coordinates of the pixel relative to the projection
         // Inverse the projectionRow to project everything upside down
         size_t projectionRow = pRasterGeom_->getTransformedY(inxCol, inxRow);
         size_t projectionColumn = pRasterGeom_->getTransformedX(inxCol, inxRow);
         // Copy colors from the raster to the projection
         ProjectedPixel* pPixel = pImageData_ - projectionRow * bitmapWidth_ + projectionColumn;
         pPixel->r += pRasterGeom_->getRed(inxCol, inxRow);
         pPixel->g += pRasterGeom_->getGreen(inxCol, inxRow);
         pPixel->b += pRasterGeom_->getBlue(inxCol, inxRow);
         pPixel->counter++;
      }
}

void WinRasterRenderer::projectPixelsUpsideDown_()
{
size_t prmy = pRasterGeom_->getMinTransformedY();
size_t prmx = pRasterGeom_->getMinTransformedX();
size_t rasterRows = pRasterGeom_->rasterHeight();
size_t rasterColumns = pRasterGeom_->rasterWidth();
size_t projectionRowPreCalc = bitmapHeight_ - 1 + prmy;
ProjectedPixel* pImageData = getImageData_() - prmx + projectionRowPreCalc * bitmapWidth_;

   UpsideDownProjector udp;
   udp.init(rasterColumns, bitmapWidth_, pRasterGeom_.get(), pImageData);
   size_t numThreads{ std::thread::hardware_concurrency() };
   // If there are no concurrent threads available or the size of the image
   // is relatively small then do the calculation in the main thread.
   if (numThreads == 0 || numThreads > size_t(rasterRows))
      udp(0, rasterRows);
   else
   {
      size_t chunkHeight = rasterRows / numThreads;
      std::vector<std::thread> threads;
      for (size_t inxThread = 0; inxThread < numThreads; inxThread++)
      {
         size_t startRow = inxThread * chunkHeight;
         size_t endRow = startRow + chunkHeight;

         // If this is the last chunk then its endRow value may need
         // to be adjusted.
         if (inxThread + 1 == numThreads)
            endRow = rasterRows;

         threads.push_back(std::thread(std::ref(udp), startRow, endRow));
      }
      for (std::thread& t : threads)
         t.join();
   }
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
