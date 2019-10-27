// wincubes_renderer.cpp : Defines the WinCubesRenderer class.
//

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

CubesGeometry& WinCubesRenderer::getCubes() {
	return *pCubesGeom_;
}

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
