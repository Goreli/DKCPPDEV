#ifndef cubes_geometry_hpp
#define cubes_geometry_hpp

#include "dkmrx_geometry.hpp"

class CubesGeometry
{
public:
	 CubesGeometry();
void     setSize(unsigned xdim, unsigned ydim);
void     getSize(unsigned* pxdim, unsigned* pydim);
void	 stop(void) 	{ active_ = 0; }
void	 cont(void)	{ active_ = 1; }

mTransformable& getCube(int i, int j) { return cubes_[i][j]; }
int	 act(void);

private:
	int     act1_(void);
	int		inxStepAct1_;
	int     act2_(void);
	double	dParamAct2_;
	int     act3_(void);
	double	dParamAct3_;
	int     act4_(void);
	double	dParamAct4_;
	int     act5_(void);
	double	dParamAct5_;

	int inxAct_;

typedef int (CubesGeometry::*ACTPTR)(void);
	ACTPTR acts_[6] = {&CubesGeometry::act1_, &CubesGeometry::act2_, 
		&CubesGeometry::act3_, &CubesGeometry::act4_, &CubesGeometry::act5_,
		nullptr};
	mTransformable cubes_[9][4];
	matrix centre_[9];
	mPoint origin_, scrCntr_;
	mLine diag_;
	double radius_;
	int active_;
	unsigned xdim_;
	unsigned ydim_;
};  

#endif // cubes_geometry_hpp
