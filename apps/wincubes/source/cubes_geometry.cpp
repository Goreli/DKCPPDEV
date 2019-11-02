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

#include "cubes_geometry.hpp"

using namespace dkmrx;

const double myPi = 3.141592653589793238463;

CubesGeometry::CubesGeometry(void):radius_(0.0), xdim_(0), ydim_(0),
	inxStepAct1_(0), dParamAct2_(0.0), dParamAct3_(0.0),
	dParamAct4_(1.0), dParamAct5_(0.0), inxAct_(0)
{
	acts_[0] = &CubesGeometry::act1_;
	acts_[1] = &CubesGeometry::act2_;
	acts_[2] = &CubesGeometry::act3_;
	acts_[3] = &CubesGeometry::act4_;
	acts_[4] = &CubesGeometry::act5_;
	acts_[5] = nullptr;
	active_	 = 1;

real init[8][3] =
{
	{ -0.5, -0.5, -0.5 },
	{  0.5, -0.5, -0.5 },
	{  0.5,  0.5, -0.5 },
	{ -0.5,  0.5, -0.5 },
	{ -0.5, -0.5,  0.5 },
	{  0.5, -0.5,  0.5 },
	{  0.5,  0.5,  0.5 },
	{ -0.5,  0.5,  0.5 }
};

matrix mrx(8, 3);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			mrx[i][j] = init[i][j];
	mrx.toHG();
	cubes_[0][0] = mrx;

mTransformer t;
	t.scale(1.0 / 2.7, 1.0 / 2.7, 1.0 / 2.7, origin_);
	mrx *= t;

	for (int i = 1; i <= 8; i++)
	{
		cubes_[i][0] = mrx;
		t.reset();
		t.translate(init[i - 1][0] * 2, init[i - 1][1] * 2, init[i - 1][2] * 2, origin_);
		cubes_[i][0].transform(t);
	}
}

void CubesGeometry::setSize(unsigned xdim, unsigned ydim)
{
unsigned minDim;
double tmp_scale;
matrix temp(0.0,1,3);
mTransformer kit;

	xdim_ = xdim;
	ydim_ = ydim;

	mPoint tmpScrCntr(xdim / 2, ydim / 2);
	scrCntr_= tmpScrCntr;
	mLine tmpDiag(origin_, scrCntr_, -origin_.distance(scrCntr_), 0);
	diag_ = tmpDiag;
	minDim = xdim;
	if( minDim > ydim ) minDim = ydim;
	radius_ = minDim/3.5;
	tmp_scale = minDim/10.0;
	for( int i=0; i<9; i++)
	{
		kit.scale(tmp_scale, tmp_scale, tmp_scale, origin_);
		kit.rotateX(myPi / 12);
		kit.rotateY(myPi / 12);
		kit.rotateZ(myPi / 12);
		kit.translate(scrCntr_,origin_);
		cubes_[i][1] = cubes_[i][0];
		cubes_[i][1].transform( kit );
		cubes_[i][3] = cubes_[i][1];
		kit.reset();
		for(int j=0;j<3;j++)
		  temp[0][j]=(float)((cubes_[i][1][0][j]+cubes_[i][1][6][j])/2.0);
		centre_[i]=temp;
	}

	dParamAct3_ = -radius_;
	dParamAct5_ = -radius_;
}
void CubesGeometry::getSize(unsigned* pxdim, unsigned* pydim)
{
	*pxdim = xdim_;
	*pydim = ydim_;
}

int CubesGeometry::act(void)
{
	if( acts_[inxAct_] == nullptr || !active_ )
		return 0;
	inxAct_ +=(this->*acts_[inxAct_])();
		return 1;
}

int CubesGeometry::act1_(void)
{
const int top=360*6;
double w1,w2,w3;
mPoint unitVectorX, unitVectorY, unitVectorZ;
mTransformer kit;

	unitVectorX.x(1);
	unitVectorY.y(1);
	unitVectorZ.z(1);
	w3=2*(w2=2*(w1=4 * myPi / top));
	double scale = 1.0 - pow(1.0 - (double)inxStepAct1_/(double)top, 0.9);
	kit.rotate(inxStepAct1_ *w1,unitVectorZ,scrCntr_);
	kit.rotate(inxStepAct1_ *w2,unitVectorY,scrCntr_);
	kit.rotate(inxStepAct1_ *w3,unitVectorX,scrCntr_);
	kit.scale(scale,scale,scale,scrCntr_);
	for(int j=0; j<9; j++)
	{
		cubes_[j][2] = cubes_[j][1];
	  	cubes_[j][2].transform( kit );
	}
	inxStepAct1_++;
	if(inxStepAct1_ == top ) return 1;
	return 0;
}
int CubesGeometry::act2_(void)
{
mTransformer t;

	t.translate( diag_.point(dParamAct2_),scrCntr_ );
	for(int i=0; i<9; i++)
	{
	  cubes_[i][2] = cubes_[i][1];
          cubes_[i][2].transform( t );
	}
	dParamAct2_ -=0.5;
	if(dParamAct2_ < -radius_ ) return 1;
	return 0;
}
int CubesGeometry::act3_(void)
{
mTransformer kit;

	double p = 1.0-pow(dParamAct3_ /radius_,2.0);
	double scale = (0.5-p)/0.5;
	kit.translate( diag_.point(dParamAct3_),scrCntr_ );
	cubes_[0][2] = cubes_[0][1];
	cubes_[0][2].transform( kit );
	for(int i=1;i<9;i++)
	{
		kit.scale(scale, scale, scale, centre_[i]);
		kit.translate((centre_[0]-centre_[i])*p,origin_ );
		cubes_[i][2] = cubes_[i][1];
		cubes_[i][2].transform( kit );
	}
	dParamAct3_ +=0.25;
	if(dParamAct3_ > radius_ ) return 1;
	return 0;
}
int CubesGeometry::act4_(void)
{
mTransformer kit;

	kit.translate( diag_.point(radius_),scrCntr_ );
	kit.scale(dParamAct4_, dParamAct4_, dParamAct4_,scrCntr_);
	for(int i=0;i<9;i++)
	{
	   cubes_[i][2] = cubes_[i][1];
	   cubes_[i][2].transform( kit );
	}
	dParamAct4_ -=0.5/radius_;
	if(dParamAct4_ < -1.0 )
	   return 1;
	return 0;
}
int CubesGeometry::act5_(void)
{
mTransformer kit;

	double p = 1.0-pow(dParamAct5_ /radius_,2.0);
	double scale = (0.5-p)/0.5;
	kit.translate( diag_.point(dParamAct5_),scrCntr_ );
	cubes_[0][2] = cubes_[0][1];
	cubes_[0][2].transform( kit );
	for(int i=1;i<9;i++)
	{
		mTransformer kit1;
		kit1.scale(scale, scale, scale, centre_[i]);
		kit1.translate((centre_[0]-centre_[i])*p,origin_ );
		kit1.concatenate( kit );
		cubes_[i][2] = cubes_[i][1];
		cubes_[i][2].transform( kit1 );
	}
	dParamAct5_ +=0.25;
	if(dParamAct5_ > radius_ ) return 1;
	return 0;
}

