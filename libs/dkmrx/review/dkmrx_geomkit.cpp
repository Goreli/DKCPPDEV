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

// All coordinates are considered in the morld (right) coordinate system
#include "dkmrx_geomkit.hpp"
#include <stdlib.h>
#include <math.h>
#include "dkmrx_error.hpp"

gPoint::gPoint(double x, double y, double z, double m)
	: matrix(1,4)
{
register real *ptr = (*this)[0];

	*(ptr++) = (real) (x*m);
	*(ptr++) = (real) (y*m);
	*(ptr++) = (real) (z*m);
	*ptr     = (real) m;
}
gPoint::gPoint(gPoint& point) : matrix(point) {}

double gPoint::distance(gPoint& point)
{
	mError::set();
	if( point.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Argument is not homogenious","gPoint::distance");
		if( point.status() == STATUS::TEMPORARY ) delete &point;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return 0.0;
	}
	if( this->columns() != 4 )
	{
		mError::set( MERR_WRONG_THIS_OBJECT, 4 );
		mError::message("This object is not homogenious","gPoint::distance");
		if( point.status() == STATUS::TEMPORARY ) delete &point;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return 0.0;
	}
	double dist = pow
			(
				pow(this->x() - point.x(), 2.0) +
				pow(this->y() - point.y(), 2.0) +
				pow(this->z() - point.z(), 2.0),
				0.5
			);
	if( point.status() == STATUS::TEMPORARY ) delete &point;
	if( this->status() == STATUS::TEMPORARY ) delete this;
	return dist;
}

void   gPoint::w(double M)
{
double temp=M/(*this)[0][3];
register real *ptr = (*this)[0];

	*(ptr++) *= (real) temp;
	*(ptr++) *= (real) temp;
	*(ptr++) *= (real) temp;
	*ptr      = (real) M;
	if( this->status() == STATUS::TEMPORARY ) delete this;
}

gLine::gLine(gPoint& point1, gPoint& point2, double par1, double par2)
	:matrix(2,4)
{
	mError::set();
matrix pars(2,2);
	if( point1.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Wrong first argument","gLine::gLine(,,,)");
		if( point1.status() == STATUS::TEMPORARY ) delete &point1;
		if( point2.status() == STATUS::TEMPORARY ) delete &point2;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return;
	}
	if( point2.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 2 );
		mError::message("Wrong second argument","gLine::gLine(,,,)");
		if( point1.status() == STATUS::TEMPORARY ) delete &point1;
		if( point2.status() == STATUS::TEMPORARY ) delete &point2;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return;
	}
	pars[0][0]    = (real) par1;
	pars[1][0]    = (real) par2;
	pars[0][1]    = pars[1][1] = 1.0;
/*
	(*this)[0][0] = point1[0][0];
	(*this)[0][1] = point1[0][1];
	(*this)[0][2] = point1[0][2];
	(*this)[0][3] = point1[0][3];
	(*this)[1][0] = point2[0][0];
	(*this)[1][1] = point2[0][1];
	(*this)[1][2] = point2[0][2];
	(*this)[1][3] = point2[0][3];
	*this /= pars;
*/
matrix temp(2,4);
matrix temp1;
	temp[0][0] = point1[0][0];
	temp[0][1] = point1[0][1];
	temp[0][2] = point1[0][2];
	temp[0][3] = point1[0][3];
	temp[1][0] = point2[0][0];
	temp[1][1] = point2[0][1];
	temp[1][2] = point2[0][2];
	temp[1][3] = point2[0][3];
	temp1 = temp/pars;
	(*this)[0][0] = temp1[0][0];
	(*this)[0][1] = temp1[0][1];
	(*this)[0][2] = temp1[0][2];
	(*this)[0][3] = temp1[0][3];
	(*this)[1][0] = temp1[1][0];
	(*this)[1][1] = temp1[1][1];
	(*this)[1][2] = temp1[1][2];
	(*this)[1][3] = temp1[1][3];

	if( point1.status() == STATUS::TEMPORARY ) delete &point1;
	if( point2.status() == STATUS::TEMPORARY ) delete &point2;
	if( this->status() == STATUS::TEMPORARY ) delete this;
}
gLine::gLine(gLine& line):matrix(line) {}
gLine::gLine(void):matrix(1.0,2,4) {}

gPoint& gLine::point(double par)
{
	mError::set();
matrix *pnt = new matrix;

	if ( pnt == NULL )
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","gLine::point");
	    return *((gPoint*)NULL);
	}

matrix temp(1,2);

	temp[0][0] = (real) par;
	temp[0][1] = 1;
	*pnt = temp * *this;
	pnt->status(STATUS::TEMPORARY);
	if( this->status() == STATUS::TEMPORARY ) delete this;
	return *((gPoint*)pnt);
}

geomKit::geomKit(void):matrix(4,4)
{
 (*this)[0][0]=1.0; (*this)[0][1]=0.0; (*this)[0][2]=0.0; (*this)[0][3]=0.0;
 (*this)[1][0]=0.0; (*this)[1][1]=1.0; (*this)[1][2]=0.0; (*this)[1][3]=0.0;
 (*this)[2][0]=0.0; (*this)[2][1]=0.0; (*this)[2][2]=1.0; (*this)[2][3]=0.0;
 (*this)[3][0]=0.0; (*this)[3][1]=0.0; (*this)[3][2]=0.0; (*this)[3][3]=1.0;
}
geomKit::geomKit(geomKit& kit):matrix(kit) {}
void geomKit::rotate(double through,matrix& about, matrix& putFrom )
{
	*this *= geomKit::rotation(through, about, putFrom);
}
void geomKit::rotateX(double through)
{
	*this *= geomKit::rotationX(through);
}
void geomKit::rotateY(double through)
{
	*this *= geomKit::rotationY(through);
}
void geomKit::rotateZ(double through)
{
	*this *= geomKit::rotationZ(through);
}
void geomKit::translate(matrix& to, matrix& putFrom )
{
	*this *= geomKit::translation(to, putFrom );
}
void geomKit::translate(double x,double y,double z, matrix& putFrom )
{
	*this *= geomKit::translation(x, y, z, putFrom );
}
void geomKit::scale(double Kx,double Ky,double Kz, matrix& about )
{
	*this *= geomKit::scaling(Kx, Ky, Kz, about );
}
void geomKit::reset(void)
{
 (*this)[0][0]=1.0; (*this)[0][1]=0.0; (*this)[0][2]=0.0; (*this)[0][3]=0.0;
 (*this)[1][0]=0.0; (*this)[1][1]=1.0; (*this)[1][2]=0.0; (*this)[1][3]=0.0;
 (*this)[2][0]=0.0; (*this)[2][1]=0.0; (*this)[2][2]=1.0; (*this)[2][3]=0.0;
 (*this)[3][0]=0.0; (*this)[3][1]=0.0; (*this)[3][2]=0.0; (*this)[3][3]=1.0;
}


geomKit& geomKit::rotation(double through,matrix& about,matrix& putFrom)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 3 );
    mError::message("Wrong third argument","geomKit::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }
  if ( about[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 2 );
    mError::message("Wrong second argument","geomKit::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }

  geomKit *r = new geomKit;
  if ( (*r)[0] == NULL  )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","geomKit::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    r->status(STATUS::TEMPORARY);
    return *r;
  }
//Init coefficients
  double a,b,c,t1,t2,t3;
  a=about[0][0];   b=about[0][1];   c=about[0][2];
  t1=putFrom[0][0]; t2=putFrom[0][1]; t3=putFrom[0][2];
//Normalization
  double v = a*a+b*b+c*c;
  if ( v == 0.0 )
  {
    mError::set( MERR_ZERO_LENGTH );
    mError::message("Do not knom hom to rotate about a zero length vector","geomKit::rotation");
    r->empty();
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    r->status(STATUS::TEMPORARY);
    return *r;
  }
  v=pow(v,0.5);
  a/=v; b/=v; c/=v;
//Prepare coefficients
  double b_2=b*b;
  double c_2=c*c;
  double v_2 = b_2+c_2;
  double z1 = cos(through);
  double z2 = sin(through);
  double Z1  = 1-z1;
  double ABZ1  = a*b*Z1;
  double ACZ1  = a*c*Z1;
  double BCZ1  = b*c*Z1;
  double CZ2 = c*z2;
  double BZ2 = b*z2;
  double AZ2 = a*z2;

(*r)[0][0] = (real)(v_2*z1 + a*a);
(*r)[0][1] = (real)(ABZ1  - CZ2);
(*r)[0][2] = (real)(BZ2   + ACZ1);
(*r)[0][3] = 0.0;
(*r)[1][0] = (real)(CZ2   + ABZ1);
(*r)[1][1] = (real)(z1     + b_2*Z1);
(*r)[1][2] = (real)(-AZ2  + BCZ1);
(*r)[1][3] = 0.0;
(*r)[2][0] = (real)(-BZ2  + ACZ1);
(*r)[2][1] = (real)(AZ2   + BCZ1);
(*r)[2][2] = (real)(z1     + c_2*Z1);
(*r)[2][3] = 0.0;
(*r)[3][0] = (real)( -t2*CZ2 + t3*BZ2  + Z1*t1*v_2 - t2*ABZ1     -  t3*ACZ1 );
(*r)[3][1] = (real)( -t3*AZ2 + t1*CZ2   - t1*ABZ1  - Z1*t2*(b_2-1) - t3*BCZ1 );
(*r)[3][2] = (real)( -t1*BZ2 + t2*AZ2 - t1*ACZ1  - t2*BCZ1      - Z1*t3*(c_2-1) );
(*r)[3][3] = 1.0;

  if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
  if ( about.status() == STATUS::TEMPORARY )   delete &about;
  r->status(STATUS::TEMPORARY);
  return *r;
}

geomKit& geomKit::rotationX(double through)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","rotationX");
    return *r;
  }

  (*r)[1][1] = (*r)[2][2] = (real) cos(through);
  (*r)[2][1] = (*r)[1][2] = (real) sin(through);
  (*r)[1][2] *= (real) -1.0;
  return *r;
}

geomKit& geomKit::rotationY(double through)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","rotationY");
    return *r;
  }

  (*r)[0][0] = (*r)[2][2] = (real) cos(through);
  (*r)[2][0] = (*r)[0][2] = (real) sin(through);
  (*r)[2][0] *= (real) -1.0;
  return *r;
}

geomKit& geomKit::rotationZ(double through)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","rotationZ");
    return *r;
  }

  (*r)[0][0] = (*r)[1][1] = (real) cos(through);
  (*r)[1][0] = (*r)[0][1] = (real) sin(through);
  (*r)[0][1] *= (real) -1.0;
  return *r;
}

geomKit& geomKit::translation(matrix& to, matrix& putFrom)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 2 );
    mError::message("Wrong second argument","geomKit::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( to.status() == STATUS::TEMPORARY )      delete &to;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }
  if ( to[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 1 );
    mError::message("Wrong first argument","geomKit::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( to.status() == STATUS::TEMPORARY )      delete &to;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }

  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","translation");
    return *r;
  }

  (*r)[3][0] = (real) ( to[0][0] - putFrom[0][0] );
  (*r)[3][1] = (real) ( to[0][1] - putFrom[0][1] );
  (*r)[3][2] = (real) ( to[0][2] - putFrom[0][2] );

  if ( to.status()== STATUS::TEMPORARY ) delete &to;
  if ( putFrom.status()== STATUS::TEMPORARY ) delete &putFrom;
  return *r;
}

geomKit& geomKit::translation(double x,double y,double z, matrix& putFrom)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 4 );
    mError::message("Wrong fourth argument","geomKit::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }

  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","translation");
    return *r;
  }

  (*r)[3][0] = (real) ( x-putFrom[0][0] );
  (*r)[3][1] = (real) ( y-putFrom[0][1] );
  (*r)[3][2] = (real) ( z-putFrom[0][2] );

  if ( putFrom.status()== STATUS::TEMPORARY ) delete &putFrom;
  return *r;
}

geomKit& geomKit::scaling(double Kx,double Ky,double Kz, matrix& about)
//Milliam M. Nemman, Robert F. Sproull;
//Principles of interactive computer graphics, McGRAM-HILL
//Chapter22:
{
  mError::set();
  if ( about[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 4 );
    mError::message("Wrong fourth argument","geomKit::scaling");
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    geomKit *r = new geomKit;
    r->status(STATUS::TEMPORARY);
    return *r;
  }

  geomKit *r = new geomKit;
  r->status(STATUS::TEMPORARY);
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","scaling");
    return *r;
  }

  (*r)[0][0] = (real) Kx;
  (*r)[1][1] = (real) Ky;
  (*r)[2][2] = (real) Kz;
  (*r)[3][0] = (real) ( about[0][0]*(1.0-Kx) );
  (*r)[3][1] = (real) ( about[0][1]*(1.0-Ky) );
  (*r)[3][2] = (real) ( about[0][2]*(1.0-Kz) );
  (*r)[3][3] = (real) 1.0;

  if ( about.status()== STATUS::TEMPORARY ) delete &about;
  return *r;
}

