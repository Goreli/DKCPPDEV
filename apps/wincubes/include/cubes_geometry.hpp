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
