/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#ifndef winraster_renderer_hpp
#define winraster_renderer_hpp

#define STRICT
#define WIN64_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <memory>
#include "multitimer.hpp"

#include "upside_down_projector.hpp"
#include "bitmap_composer.hpp"
#include "multi_threaded_driver.hpp"


struct ProjectedPoint
{
   unsigned long b;
   unsigned long g;
   unsigned long r;
   unsigned long counter;
};

class RasterGeometry;

class WinRasterRenderer
{
public:
	WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd);
	~WinRasterRenderer();
	void backgroundJob(void);
	void setSize(unsigned winWidth, unsigned winHeight);

private:
	void initProjectionBuffer_(void);
	void initBitmapBuffer_(RECT& rectBoundingBox);
	void drawBitmap_(RECT& rectBoundingBox);

	size_t		iProjectionWidth_;
	size_t		iProjectionHeight_;

	std::unique_ptr<ProjectedPoint[]> pProjectionBuffer_;
	// The max size of the buffer encountered so far.
	// Don't reallocate the buffer if the max size is
	// sufficient to accommodate the current data.
	size_t iLastSizePB_;

	size_t	numBytesInRow_;

	std::unique_ptr<unsigned char[]> pBitmapBuffer_;
	// The max size of the buffer encountered so far.
	// Don't reallocate the buffer if the max size is
	// sufficient to accommodate the current data.
	size_t iLastSizeBB_;

	RECT rectLast_;
	MultiTimer mt_;

	HWND hwnd_;
	HDC  hdc_;
	std::unique_ptr<RasterGeometry> pRasterGeom_;
	COLORREF colorRefBackground_;

	size_t iLeftMargin_;
	size_t iTopMargin_;
	size_t iRightMargin_;
	size_t iBottomMargin_;
	size_t iBitmapWidth_;
	size_t iBitmapHeight_;

	UpsideDownProjector projector_;
	BitmapComposer composer_;
	MultiThreadedDriver driverMT_;
};

#endif // winraster_renderer_hpp
