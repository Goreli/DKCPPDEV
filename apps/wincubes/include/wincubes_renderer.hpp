#ifndef wincubes_renderer_hpp
#define wincubes_renderer_hpp

#define STRICT
#define WIN64_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <memory>

class CubesGeometry;
class mTransformable;

class WinCubesRenderer {
private:
	POINT vertices_[16] { 0 };
	// The following array contains numbers of vertices that defines the edge drawing order.
	const int edgeDrawingOrder_[16] { 0, 1, 2, 3, 0, 4, 5, 6, 7, 4, 5, 1, 2, 6, 7, 3 };

	HWND hWnd_ { nullptr };
	HDC hdc_ { nullptr };

	const LOGPEN logPenBlack_ { PS_SOLID, {1, 0}, RGB(0, 0, 0) };
	const LOGPEN logPenWhite_ { PS_SOLID, {1, 0}, RGB(255, 255, 255) };
	const LOGPEN logPenHighlight_ { PS_SOLID, {1, 0}, RGB(255, 0, 0) };

	HPEN hPenWhite_ { nullptr };
	HPEN hPenBlack_ { nullptr };
	HPEN hPenHighlight_ { nullptr };

	DWORD dwDelayMS_ { 10 };

	std::unique_ptr<CubesGeometry> pCubesGeom_;

	void renderAllCubes_(void);
	void renderCube_(mTransformable&);
	void erasePreviousCubes_();

public:
	WinCubesRenderer(HWND hWnd);
	~WinCubesRenderer();
	void setSize(unsigned xdim, unsigned ydim);
	void background_job(void);
	void highlight(WORD CubeNumber);
	void setDelayMS(DWORD dwDelayMS);
	void stop();
	void cont();
	void restart();
};

#endif // wincubes_renderer_hpp
