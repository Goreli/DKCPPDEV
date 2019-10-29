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

#ifndef wincubes_renderer_hpp
#define wincubes_renderer_hpp

#define STRICT
#define WIN64_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <memory>

// Forward references:
class CubesGeometry;
namespace dkmrx {
	class mTransformable;
}

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
	void renderCube_(dkmrx::mTransformable&);
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
