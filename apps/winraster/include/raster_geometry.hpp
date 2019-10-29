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

#ifndef raster_geometry_hpp
#define raster_geometry_hpp

// Forward declarations:
namespace dkmrx {
	class mPoint;
	class mLine;
	class mTransformable;
}

typedef unsigned char uch;

class RasterGeometry
{
public:
	RasterGeometry(wchar_t* colorFileName);
	~RasterGeometry();

void setSize(unsigned winWidth, unsigned winHeight);
void getSize(unsigned* pWinWidth, unsigned* pWinHeight);
void nextFrame(void);

int	getTransformedX(int x, int y);
int	getTransformedY(int x, int y);

uch getRed(int x, int y);
uch getGreen(int x, int y);
uch getBlue(int x, int y);

int	rasterWidth( void ){ return rasterWidth_; }
int	rasterHeight( void ){ return rasterHeight_; }
int	getMaxTransformedX( void );
int	getMaxTransformedY( void );
int	getMinTransformedX( void );
int	getMinTransformedY( void );

private:
	void readColors_(wchar_t* colorFileName);
	void setInitialCoords_( void );

	struct RGBPixel_;
	std::unique_ptr<RGBPixel_[]> pixels_;

	std::unique_ptr<dkmrx::mTransformable> initialCoords_;
	std::unique_ptr<dkmrx::mTransformable> transformedCoords_;

	int rasterWidth_;
	int rasterHeight_;
	double rasterRadius_;
	std::unique_ptr<dkmrx::mPoint> rasterCentre_;

	int frameCounter_;

// Size dependant items
	double windowRadius_;
	std::unique_ptr<dkmrx::mPoint> windowCentre_;
	std::unique_ptr<dkmrx::mLine> windowDiag_;
};

#endif // raster_geometry_hpp
