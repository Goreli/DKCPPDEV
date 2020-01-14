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

#ifndef raster_geometry_hpp
#define raster_geometry_hpp

#include <memory>

// Forward declarations:
namespace dkmrx {
	class mPoint;
	class mLine;
	class mTransformer;
	class matrix;
	class MultiThreadedDriver;
}

typedef unsigned char uch;

class RasterGeometry
{
public:
	RasterGeometry(wchar_t* colorFileName);
	~RasterGeometry();

void setSize(unsigned winWidth, unsigned winHeight);
void getSize(unsigned* pWinWidth, unsigned* pWinHeight);
//void nextFrame(void);
void setupTransformer(void);
void transformInitialCoords(dkmrx::MultiThreadedDriver*);

size_t	getTransformedX(size_t x, size_t y) noexcept;
size_t	getTransformedY(size_t x, size_t y) noexcept;

uch getRed(size_t x, size_t y) noexcept;
uch getGreen(size_t x, size_t y) noexcept;
uch getBlue(size_t x, size_t y) noexcept;

size_t	rasterWidth( void ){ return rasterWidth_; }
size_t	rasterHeight( void ){ return rasterHeight_; }
size_t	getMaxTransformedX( void ) noexcept;
size_t	getMaxTransformedY( void ) noexcept;
size_t	getMinTransformedX( void ) noexcept;
size_t	getMinTransformedY( void ) noexcept;

private:
	void readColors_(wchar_t* colorFileName);
	void setInitialCoords_( void );

	struct RGBPixel_;
	std::unique_ptr<RGBPixel_[]> pixels_;

	std::unique_ptr<dkmrx::matrix> initialCoords_;
	std::unique_ptr<dkmrx::mTransformer> transformer_;
	std::unique_ptr<dkmrx::matrix> transformedCoords_;

   size_t rasterWidth_;
   size_t rasterHeight_;
	double rasterRadius_;
	std::unique_ptr<dkmrx::mPoint> rasterCentre_;

   size_t frameCounter_;

// Size dependant items
	double windowRadius_;
	std::unique_ptr<dkmrx::mPoint> windowCentre_;
	std::unique_ptr<dkmrx::mLine> windowDiag_;
};

#endif // raster_geometry_hpp
