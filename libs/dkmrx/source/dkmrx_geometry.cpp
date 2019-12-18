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
// Seen some interesting geometric calculations here:
//	https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors

// All coordinates are considered in the morld (right) coordinate system
#include <cmath>
#include "dkmrx_geometry.hpp"

using namespace dkmrx;

mPoint::mPoint(real x, real y, real z, real w)
	: mTransformable(1)
{
real *ptr = (*this)[0];

	*(ptr++) = x*w;
	*(ptr++) = y*w;
	*(ptr++) = z*w;
	*ptr     = w;
}
mPoint::mPoint(const mPoint& point) : mTransformable(point) {}

double mPoint::distance(const mPoint& point) const
{
	_validate((*this)[0], point[0], true, "mPoint::distance(const mPoint&) const");

	double dist = std::pow
			(
				std::pow(this->x() - point.x(), 2.0) +
				std::pow(this->y() - point.y(), 2.0) +
				std::pow(this->z() - point.z(), 2.0),
				0.5
			);
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
}

mLine::mLine(const mPoint& point1, const mPoint& point2, real par1, real par2)
	:mTransformable(2)
{
	_validate(point1[0], point2[0], true, "mLine::mLine(const mPoint&, const mPoint&, real, real)");

matrix pars{
	{par1, 1},
	{par2, 1}
};

	(*this)[0][0] = point1[0][0];
	(*this)[0][1] = point1[0][1];
	(*this)[0][2] = point1[0][2];
	(*this)[0][3] = point1[0][3];
	(*this)[1][0] = point2[0][0];
	(*this)[1][1] = point2[0][1];
	(*this)[1][2] = point2[0][2];
	(*this)[1][3] = point2[0][3];
	*this /= pars;
}
mLine::mLine(const mLine& line):mTransformable(line) {}
mLine::mLine(void):mTransformable(1,2) {}

mPoint mLine::point(real par) const
{
	mPoint pnt;
	matrix temp{ {par, 1} };

	matrix& mPnt = pnt;
	mPnt = temp * *this;

	return pnt;
}
