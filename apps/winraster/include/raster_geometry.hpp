#ifndef raster_geometry_hpp
#define raster_geometry_hpp

/*
HISTORY:
	28/May/2011 - removed the maxFrameNum argument from nextFrame.
	14/Oct/2019 - Applied minor changes to compile it as a 64 bit module.
		Renamed the module from rastergr.hpp to raster_geometry.hpp.
  */

class mPoint;
class mLine;
class mTransformable;

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

	std::unique_ptr<mTransformable> initialCoords_;
	std::unique_ptr<mTransformable> transformedCoords_;

	int rasterWidth_;
	int rasterHeight_;
	double rasterRadius_;
	std::unique_ptr<mPoint> rasterCentre_;

	int frameCounter_;

// Size dependant items
	double windowRadius_;
	std::unique_ptr<mPoint> windowCentre_;
	std::unique_ptr<mLine> windowDiag_;
};

#endif // raster_geometry_hpp
