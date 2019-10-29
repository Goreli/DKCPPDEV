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

#include <fstream>
#include <cmath>
#include "dkmrx_geometry.hpp"
#include "raster_geometry.hpp"

using namespace dkmrx;
using std::ios;
using std::fstream;

const double myPi = 3.14159265358979323846;

struct RasterGeometry::RGBPixel_
{
	RGBPixel_(void): r(0), g(0), b(0) {}
	uch b;
	uch g;
	uch r;
};

RasterGeometry::RasterGeometry(wchar_t* colorFileName)
	: pixels_(nullptr), initialCoords_(nullptr), transformedCoords_(nullptr),
	rasterWidth_(0), rasterHeight_(0), rasterRadius_(0.0), rasterCentre_(nullptr),
	frameCounter_(0), windowRadius_(0.0), windowCentre_(nullptr), 
	windowDiag_(nullptr)
{
	readColors_(colorFileName);
	setInitialCoords_();

	real	diagDeltaX = (*initialCoords_)[rasterWidth_ - 1][0] -
		(*initialCoords_)[0][0];
	real	diagDeltaY = (*initialCoords_)[rasterWidth_ * rasterHeight_ - 1][1] -
		(*initialCoords_)[0][1];
	rasterRadius_ = pow(pow(diagDeltaX, 2) + pow(diagDeltaY, 2), 0.5) / 2;
	rasterCentre_ = std::make_unique<mPoint>(0, 0);
}

// The destructor doesn't do much here since all the cleanup is done using smart pointers.
// However, it's still required to enable the forward referencing of some classes used in
// the header file of the module. 
// Without the destructor declared in the module header file the <memory> include file causes
// a compilation time error unable to figure out how the object is going to be destroyed.
// This error forces an explicit inclusion of header files that declare classes currently
// forward referenced. I don't want to sacrifice the forward referencing for the sake of
// saving a few lines of code required for the destructor.
RasterGeometry::~RasterGeometry() {}

void RasterGeometry::setSize(unsigned winWidth, unsigned winHeight)
{
	windowCentre_ = std::make_unique<mPoint>(winWidth / 2, winHeight / 2);
	windowRadius_ = (winWidth > winHeight)?winHeight/2:winWidth/2;
	windowRadius_--;
	windowDiag_ = std::make_unique<mLine>(*windowCentre_, *rasterCentre_,
		0, pow(pow(winWidth / 2, 2) + pow(winHeight / 2, 2), 0.5));
}

int RasterGeometry::getMaxTransformedX( void )
{
real maxX = (*transformedCoords_)[0][0];
real temp = (*transformedCoords_)[rasterWidth_ - 1][0];
	if( temp > maxX )
		maxX = temp;
	temp = (*transformedCoords_)[rasterWidth_ * (rasterHeight_ - 1)][0];
	if( temp > maxX )
		maxX = temp;
	temp = (*transformedCoords_)[rasterWidth_ * rasterHeight_ - 1][0];
	if( temp > maxX )
		maxX = temp;

	// Round the value.
	real delta = maxX - (int)maxX;
	real absdelta = abs(delta);
	real retVal = (absdelta < 0.5) ? maxX : maxX + delta;

	return (int)retVal;
}

int RasterGeometry::getMaxTransformedY( void )
{
real maxY = (*transformedCoords_)[0][1];
real temp = (*transformedCoords_)[rasterWidth_ - 1][1];
	if( temp > maxY )
		maxY = temp;
	temp = (*transformedCoords_)[rasterWidth_ * (rasterHeight_ - 1)][1];
	if( temp > maxY )
		maxY = temp;
	temp = (*transformedCoords_)[rasterWidth_ * rasterHeight_ - 1][1];
	if(  temp > maxY )
		maxY = temp;

	// Round the value.
	real delta = maxY - (int)maxY;
	real absdelta = abs(delta);
	real retVal = (absdelta < 0.5) ? maxY : maxY + delta;

	return (int)retVal;
}

uch	RasterGeometry::getRed(int x, int y)
{
	return pixels_[y * rasterWidth_ + x].r;
}

uch	RasterGeometry::getGreen(int x, int y)
{
	return pixels_[y * rasterWidth_ + x].g;
}

uch	RasterGeometry::getBlue(int x, int y)
{
	return pixels_[y * rasterWidth_ + x].b;
}

int RasterGeometry::getMinTransformedX( void )
{
real minX = (*transformedCoords_)[0][0];
real temp = (*transformedCoords_)[rasterWidth_ - 1][0];
	if( temp < minX )
		minX = temp;
	temp = (*transformedCoords_)[rasterWidth_ * (rasterHeight_ - 1)][0];
	if( temp < minX )
		minX = temp;
	temp = (*transformedCoords_)[rasterWidth_ * rasterHeight_ - 1][0];
	if( temp < minX )
		minX = temp;

	return static_cast<int>(std::round(minX));
}

int RasterGeometry::getMinTransformedY( void )
{
real minY = (*transformedCoords_)[0][1];
real temp = (*transformedCoords_)[rasterWidth_ - 1][1];
	if(temp < minY)
		minY = temp;
	temp = (*transformedCoords_)[rasterWidth_ * (rasterHeight_ - 1)][1];
	if( temp < minY )
		minY = temp;
	temp = (*transformedCoords_)[rasterWidth_ * rasterHeight_ - 1][1];
	if( temp < minY )
		minY = temp;

	return static_cast<int>(std::round(minY));
}

int RasterGeometry::getTransformedX(int x, int y)
{
real temp = (*transformedCoords_)[y*rasterWidth_+x][0];

	return static_cast<int>(std::round(temp));
}

int RasterGeometry::getTransformedY(int x, int y)
{
real temp = (*transformedCoords_)[y*rasterWidth_+x][1];

	return static_cast<int>(std::round(temp));
}

void RasterGeometry::readColors_(wchar_t* colorFileName)
{
matrix mrxColorData;
	if( colorFileName )
	{
		fstream colorFile( colorFileName, ios::in );
		colorFile >> mrxColorData;
	}
	else
	{	// Create an artificial image just to keep going.
	int rows = 90;
	int cols = 3*rows;
	matrix tempMatrix( 0.0, rows, cols );
	    for ( int r = 0; r < rows; r++ )
		for( int c = 0; c < cols; c++ )
			if( c < cols/3 )
				tempMatrix[r][c] = 0xFF;
			else if( c < 2*cols/3 )
				tempMatrix[r][c] = 0xFF00;
			else
				tempMatrix[r][c] = 0xFF0000;

		mrxColorData = tempMatrix;
	//fstream tempFile( "temp.clr", ios::out );
		//tempFile << *colorMatrix;
	}

	rasterWidth_ = mrxColorData.columns();
	rasterHeight_ = mrxColorData.rows();
	pixels_ = std::make_unique<RGBPixel_[]>(rasterHeight_ * rasterWidth_);
	for( int i=0; i < rasterHeight_; i++ )
	 for( int j=0; j < rasterWidth_; j++ )
	 {
	 int index = i*rasterWidth_ + j;
		 pixels_[index].r = (unsigned)(mrxColorData[i][j]) & 0xFF;
		 pixels_[index].g = ((unsigned)(mrxColorData[i][j]) & 0xFF00) >> 8;
		 pixels_[index].b = ((unsigned)(mrxColorData[i][j]) & 0xFF0000) >> 16;
	 }
}

void RasterGeometry::setInitialCoords_( void )
{
double halfWidth = rasterWidth_/2;
double halfHeight = rasterHeight_/2;

	initialCoords_ = std::make_unique<mTransformable>(rasterHeight_ * rasterWidth_);
	for( int i=0; i < rasterHeight_; i++ )
	 for( int j=0; j < rasterWidth_; j++ )
	 {
	 int index = i*rasterWidth_ + j;
		initialCoords_->operator [](index)[0] = (unsigned __int64)1+j - halfWidth; // Set x coordinate
		initialCoords_->operator [](index)[1] = (unsigned __int64)1+i - halfHeight; // Set y coordinate
		initialCoords_->operator [](index)[2] = 0; // Set z coordinate
	 }
}

void RasterGeometry::nextFrame( void )
{
double w1,w2,w3;
	w2 = 2 * myPi / 360; 
	w1 = 4 * w2; 
	w3 = w1 * (windowRadius_/rasterRadius_ - 1);

matrix aboutZ(0.0,1,3);
	aboutZ[0][0]=0; aboutZ[0][1]=0; aboutZ[0][2]=1;
matrix aboutX(0.0,1,3);
	aboutX[0][0]=1; aboutX[0][1]=0; aboutX[0][2]=0;

mTransformer trans;
	trans.rotate( -w1*frameCounter_/2, aboutZ, *rasterCentre_);

mPoint dest( -windowCentre_->x(), -rasterRadius_ );
	trans.translate( *rasterCentre_, dest);

mPoint putFrom( dest.x(), -dest.y() + rasterRadius_ );
	trans.rotate( -w2*frameCounter_/2, aboutX, putFrom );

/*
mTransformer tempTrans;
	tempTrans.rotate( PI/2, aboutZ, dest );
mLine newAbout(*windowDiag_);
	newAbout.transform( tempTrans );

	trans.rotate( -w2*frameCounter_/6, newAbout );
*/
	trans.rotate(  -w3*frameCounter_/2, aboutZ, *windowCentre_ );
	transformedCoords_ = std::make_unique<mTransformable>(*initialCoords_);
	transformedCoords_->transform( trans );

	frameCounter_++;
}
