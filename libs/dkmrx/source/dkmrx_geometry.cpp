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
#include <cmath>
#include "dkmrx_geometry.hpp"
#include "dkmrx_error.hpp"

using namespace dkmrx;

mPoint::mPoint(double x, double y, double z, double w)
	: mTransformable(1)
{
real *ptr = (*this)[0];

	*(ptr++) = (real) (x*w);
	*(ptr++) = (real) (y*w);
	*(ptr++) = (real) (z*w);
	*ptr     = (real) w;
}
mPoint::mPoint(mPoint& point) : mTransformable(point) {}

double mPoint::distance(mPoint& point)
{
	mError::set();
	if( point.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Argument is not homogenious","mPoint::distance");
		if( point.status() == STATUS::TEMPORARY ) delete &point;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return 0.0;
	}
	if( this->columns() != 4 )
	{
		mError::set( MERR_WRONG_THIS_OBJECT, 4 );
		mError::message("This object is not homogenious","mPoint::distance");
		if( point.status() == STATUS::TEMPORARY ) delete &point;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return 0.0;
	}
	double dist = std::pow
			(
				std::pow(this->x() - point.x(), 2.0) +
				std::pow(this->y() - point.y(), 2.0) +
				std::pow(this->z() - point.z(), 2.0),
				0.5
			);
	if( point.status() == STATUS::TEMPORARY ) delete &point;
	if( this->status() == STATUS::TEMPORARY ) delete this;
	return dist;
}

void   mPoint::w(double M)
{
double temp=M/(*this)[0][3];
real *ptr = (*this)[0];

	*(ptr++) *= (real) temp;
	*(ptr++) *= (real) temp;
	*(ptr++) *= (real) temp;
	*ptr      = (real) M;
	if( this->status() == STATUS::TEMPORARY ) delete this;
}

mLine::mLine(mPoint& point1, mPoint& point2, double par1, double par2)
	:mTransformable(2)
{
	mError::set();
matrix pars(2,2);
	if( point1.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Wrong first argument","mLine::mLine(,,,)");
		if( point1.status() == STATUS::TEMPORARY ) delete &point1;
		if( point2.status() == STATUS::TEMPORARY ) delete &point2;
		if( this->status() == STATUS::TEMPORARY ) delete this;
		return;
	}
	if( point2.columns() != 4 )
	{
		mError::set( MERR_WRONG_ARGUMENT, 2 );
		mError::message("Wrong second argument","mLine::mLine(,,,)");
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
mLine::mLine(mLine& line):mTransformable(line) {}
mLine::mLine(void):mTransformable(1,2) {}

mPoint& mLine::point(double par)
{
	mError::set();
matrix *pnt = new matrix;

	if ( pnt == nullptr )
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","mLine::point");
	    return *((mPoint*)nullptr);
	}

matrix temp(1,2);

	temp[0][0] = (real) par;
	temp[0][1] = 1;
	*pnt = temp * *this;
	pnt->status(STATUS::TEMPORARY);
	if( this->status() == STATUS::TEMPORARY ) delete this;
	return *((mPoint*)pnt);
}
