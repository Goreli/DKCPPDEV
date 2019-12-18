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
#include <chrono>

class RasterGeometry;

class WinRasterRenderer
{
public:
	WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd);
	~WinRasterRenderer();
	void backgroundJob(void);
	void eraseLastRect();
	void setSize(unsigned winWidth, unsigned winHeight);

private:
	HWND hwnd_;
	HDC  hdc_;
	RECT rectLast_;
   HBRUSH hBrushBG_;

	std::unique_ptr<RasterGeometry> pRasterGeom_;
	std::unique_ptr<unsigned char[]> pBitmap_;
	   
	int		bitmapWidth_;
	int		bitmapHeight_;

	COLORREF colorRefBackground_;

	void	initBitmapData_(void);
	void 	projectPixelsUpsideDown_(void);
	void 	projection2ActualBitmap_(void);
	//void 	drawBitmap_(void);

	struct ProjectedPixel_;
	ProjectedPixel_* getImageData_();


   std::chrono::duration<double> duration1_{ 0.0 };
   std::chrono::duration<double> duration2_{ 0.0 };
   std::chrono::duration<double> duration3_{ 0.0 };
   std::chrono::duration<double> duration4_{ 0.0 };
   std::chrono::duration<double> duration5_{ 0.0 };
   std::chrono::duration<double> duration6_{ 0.0 };
};

#endif // winraster_renderer_hpp
