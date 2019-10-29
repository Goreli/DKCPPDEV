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

#include "wincubes_renderer.hpp"
#include "cubes_geometry.hpp"
#include "resource.h"

WinCubesRenderer::WinCubesRenderer(HWND hWnd) {
	hWnd_ = hWnd;
	hdc_ = GetDC(hWnd);

	hPenBlack_ = CreatePenIndirect(&logPenBlack_);
	hPenWhite_ = CreatePenIndirect(&logPenWhite_);
	hPenHighlight_ = CreatePenIndirect(&logPenHighlight_);

	pCubesGeom_ = std::make_unique<CubesGeometry>();
}

WinCubesRenderer::~WinCubesRenderer() {
	ReleaseDC(hWnd_, hdc_);
	DeleteObject(hPenBlack_);
	DeleteObject(hPenWhite_);
	DeleteObject(hPenHighlight_);
}

void WinCubesRenderer::restart() {
	for (int i = 0; i < 9; i++)
	{
		// Erase old: draw using a background color
		SelectObject(hdc_, hPenBlack_);
		renderCube_(pCubesGeom_->getCube(i, 3));
	}

	unsigned xdim, ydim;
	pCubesGeom_->getSize(&xdim, &ydim);
	pCubesGeom_ = std::make_unique<CubesGeometry>();
	pCubesGeom_->setSize(xdim, ydim);
}

void WinCubesRenderer::stop() {
	pCubesGeom_->stop();
}

void WinCubesRenderer::cont() {
	pCubesGeom_->cont();
}

void WinCubesRenderer::setSize(unsigned xdim, unsigned ydim) {
	pCubesGeom_->setSize(xdim, ydim);
}

/*
CubesGeometry& WinCubesRenderer::getCubes() {
	return *pCubesGeom_;
}
*/

void WinCubesRenderer::setDelayMS(DWORD dwDelayMS) {
	dwDelayMS_ = dwDelayMS;
}

void WinCubesRenderer::background_job(void) {
	if (pCubesGeom_->act())
	{
		renderAllCubes_();
		Sleep(dwDelayMS_);
	}
}

void WinCubesRenderer::highlight(WORD CubeNumber)
{
	// Redraw all cubes
	SelectObject(hdc_, hPenWhite_);
	for (int i = 0; i < 9; i++)
		renderCube_(pCubesGeom_->getCube(i, 2));
	// Draw highlighted cube
	SelectObject(hdc_, hPenHighlight_);
	renderCube_(pCubesGeom_->getCube(CubeNumber, 2));
}

void WinCubesRenderer::renderAllCubes_(void)
{
	for (int i = 0; i < 9; i++)
	{
		// Erase old: draw using a background color
		SelectObject(hdc_, hPenBlack_);
		renderCube_(pCubesGeom_->getCube(i, 3));
		// Draw new: draw using a foreground color
		SelectObject(hdc_, hPenWhite_);
		renderCube_(pCubesGeom_->getCube(i, 2));
		// Update old
		pCubesGeom_->getCube(i, 3) = pCubesGeom_->getCube(i, 2);
	}
}

void WinCubesRenderer::renderCube_(mTransformable& cube)
{
	for (int i = 0; i < 16; i++)
	{
		vertices_[i].x = (LONG)(cube[edgeDrawingOrder_[i]][0]);
		vertices_[i].y = (LONG)(cube[edgeDrawingOrder_[i]][1]);
	}
	bool bReturnValue = Polyline(hdc_, vertices_, 16);
}
