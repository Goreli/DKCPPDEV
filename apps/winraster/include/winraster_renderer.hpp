#ifndef winraster_renderer_hpp
#define winraster_renderer_hpp

#define STRICT
#define WIN64_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>

class RasterGeometry;

class WinRasterRenderer
{
public:
	WinRasterRenderer(HWND hwnd, wchar_t* colorFileName, COLORREF crBgrnd);
	~WinRasterRenderer();
	void backgroundJob(void);
	void erasePrevRect();
	void setSize(unsigned winWidth, unsigned winHeight);

private:
	HWND hwnd_;
	HDC  hdc_;
	RECT rectPrev_;

	std::unique_ptr<RasterGeometry> pRasterGeom_;
	std::unique_ptr<unsigned char[]> pBitmap_;
	   
	int		bitmapWidth_;
	int		bitmapHeight_;

	COLORREF colorRefBackground_;

	void	initBitmapData_(void);
	void 	projectPixelsUpsideDown_(void);
	void 	projection2ActualBitmap_(void);
	void 	drawBitmap_(void);

	struct ProjectedPixel_;
	ProjectedPixel_* getImageData_();
};

#endif // winraster_renderer_hpp
