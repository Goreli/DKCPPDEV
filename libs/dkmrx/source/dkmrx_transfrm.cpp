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

// All coordinates are considered in the world (right) coordinate system.
#include <cmath>
#include <stdexcept>
#include "dkmrx_geometry.hpp"

using namespace dkmrx;

mTransformer::mTransformer(void):matrix(4,4)
{
real* values = (*this)[0];

	*values  =1;	*++values=0;	*++values=0;	*++values=0;
	*++values=0;	*++values=1;	*++values=0;	*++values=0;
	*++values=0;	*++values=0;	*++values=1;	*++values=0;
	*++values=0;	*++values=0;	*++values=0;	*++values=1;
}

mTransformer::mTransformer(const mTransformer& kit):matrix(kit) {}

void mTransformer::reset(void)
{
real* values = (*this)[0];

	*values  =1;	*++values=0;	*++values=0;	*++values=0;
	*++values=0;	*++values=1;	*++values=0;	*++values=0;
	*++values=0;	*++values=0;	*++values=1;	*++values=0;
	*++values=0;	*++values=0;	*++values=0;	*++values=1;
}

void mTransformer::rotate(real through,const matrix& mrxAbout,const matrix& mrxPutFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
const char* sFunctionSignature = "mTransformer::rotate(real, const matrix&, const matrix&)";
	_validate(mrxAbout[0], mrxPutFrom[0], true, sFunctionSignature);

	mTransformer transformer; 

	//Normalization.
  real a= mrxAbout[0][0];   real b= mrxAbout[0][1];   real c= mrxAbout[0][2];
  real v = a*a+b*b+c*c;

  if (v == 0.0)
	  throw std::runtime_error(std::string("Illegal attempt to rotate about a zero length vector ") + sFunctionSignature);
  else
  {
	v=std::pow(v,0.5);
	a/=v; b/=v; c/=v;
	// Normalization done.

	real Z1  = 1 - std::cos(through);
	real z2 = std::sin(through);

	real ABZ1  = a*b*Z1;
	real CZ2 = c*z2;
	transformer[0][1] = (real)(ABZ1 - CZ2);
	transformer[1][0] = (real)(ABZ1 + CZ2);

	real ACZ1  = a*c*Z1;
	real BZ2 = b*z2;
	transformer[0][2] = (real)(ACZ1 + BZ2);
	transformer[2][0] = (real)(ACZ1 - BZ2);

	real BCZ1  = b*c*Z1;
	real AZ2 = a*z2;
	transformer[1][2] = (real)(BCZ1 - AZ2);
	transformer[2][1] = (real)(BCZ1 + AZ2);

	real b_2 = b*b;
	real c_2 = c*c;
  	real Z1b_2 = Z1*b_2;
  	real Z1c_2 = Z1*c_2;
	real Z1B_2 = Z1-Z1b_2;
	real Z1C_2 = Z1-Z1c_2;
	real Z1v_2 = Z1b_2+Z1c_2;

	transformer[0][0] = (real)(1 - Z1v_2);
	transformer[1][1] = (real)(1 - Z1B_2);
	transformer[2][2] = (real)(1 - Z1C_2);

	real t1= mrxPutFrom[0][0]; real t2= mrxPutFrom[0][1]; real t3= mrxPutFrom[0][2];
	transformer[3][0] = (real)( t1*Z1v_2 - t2*transformer[1][0] - t3*transformer[2][0] );
	transformer[3][1] = (real)( t2*Z1B_2 - t3*transformer[2][1] - t1*transformer[0][1] );
	transformer[3][2] = (real)( t3*Z1C_2 - t1*transformer[0][2] - t2*transformer[1][2] );

	transformer[0][3] = 0.0;
	transformer[1][3] = 0.0;
	transformer[2][3] = 0.0;
	transformer[3][3] = 1.0;

	(*this) *= transformer;
  }
}

void mTransformer::rotate(real through, const mLine& about )
{
mPoint p0 = about.point(0);
mPoint p1 = about.point(1);
mPoint vector(p1.x()-p0.x(), p1.y()-p0.y(), p1.z()-p0.z());
	rotate( through, vector, p0 );
}

void mTransformer::rotateX(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mTransformer transformer;

  transformer[1][1] = transformer[2][2] = (real)std::cos(through);
  transformer[2][1] = transformer[1][2] = (real)std::sin(through);
  transformer[1][2] *= (real) -1.0;
  (*this) *= transformer;
}  

void mTransformer::rotateY(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mTransformer transformer;

  transformer[0][0] = transformer[2][2] = (real)std::cos(through);
  transformer[2][0] = transformer[0][2] = (real)std::sin(through);
  transformer[2][0] *= (real) -1.0;
  (*this) *= transformer;
}  

void mTransformer::rotateZ(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mTransformer transformer;

  transformer[0][0] = transformer[1][1] = (real)std::cos(through);
  transformer[1][0] = transformer[0][1] = (real)std::sin(through);
  transformer[0][1] *= (real) -1.0;
  (*this) *= transformer;
}  
 
void mTransformer::translate(const matrix& mrxTo, const matrix& mrxPutFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
const char* sFunctionSignature = "matrix::translate(const matrix&, const matrix&)";
	_validate(mrxTo[0], mrxPutFrom[0], true, sFunctionSignature);

	mTransformer transformer;

	transformer[3][0] = (real) (mrxTo[0][0] - mrxPutFrom[0][0] );
	transformer[3][1] = (real) (mrxTo[0][1] - mrxPutFrom[0][1] );
	transformer[3][2] = (real) (mrxTo[0][2] - mrxPutFrom[0][2] );

	(*this) *= transformer;
}

void mTransformer::translate(real x, real y, real z, const matrix& mrxPutFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
	_validate(mrxPutFrom[0], "mTransformer::translate(real,real,real, const matrix&)");
	mTransformer transformer;

	transformer[3][0] = (real) ( x- mrxPutFrom[0][0] );
	transformer[3][1] = (real) ( y- mrxPutFrom[0][1] );
	transformer[3][2] = (real) ( z- mrxPutFrom[0][2] );

	(*this) *= transformer;
}  
  
void mTransformer::scale(real Kx, real Ky, real Kz, const matrix& mrxAbout)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
	_validate(mrxAbout[0], "mTransformer::scale(real, real, real, const matrix&)");
	mTransformer transformer;

	transformer[0][0] = (real) Kx;
	transformer[1][1] = (real) Ky;
	transformer[2][2] = (real) Kz;
	transformer[3][0] = (real) (mrxAbout[0][0]*(1.0-Kx) );
	transformer[3][1] = (real) (mrxAbout[0][1]*(1.0-Ky) );
	transformer[3][2] = (real) (mrxAbout[0][2]*(1.0-Kz) );
	transformer[3][3] = (real) 1.0;
   
	(*this) *= transformer;
}  

mTransformable::mTransformable(size_t rows) : matrix(rows, 4)
{
real* element = (*this)[0] + 3;
real* top = element + rows*4;
	while( element < top )
	{
		*element = 1;
		element += 4;
	}
}

mTransformable::mTransformable(size_t rows, real init_value) : matrix(rows, 4, init_value)
{
real* element = (*this)[0] + 3;
real* top = element + rows*4;
	while( element < top )
	{
		*element = 1;
		element += 4;
	}
}

mTransformable::mTransformable(const mTransformable& t) : matrix( t ) {}

mTransformable::mTransformable( void ) : matrix() {}

void mTransformable::transform(const mTransformer& t)
{
	(*this)*=t;
}

