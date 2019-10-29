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

#ifndef dkmrx_geometry_hpp
#define dkmrx_geometry_hpp

#include "dkmrx_transfrm.hpp"

namespace dkmrx {

	class mPoint : public mTransformable
	{
	public:

		mPoint(real x = 0, real y = 0, real z = 0, real w = 1);
		mPoint(mPoint&);

		real	distance(mPoint&);
		real	x(void) { return (*this)[0][0] / (*this)[0][3]; }
		void	x(real X) { (*this)[0][0] = (real)((*this)[0][3] * X); }
		real	y(void) { return (*this)[0][1] / (*this)[0][3]; }
		void	y(real Y) { (*this)[0][1] = (real)((*this)[0][3] * Y); }
		real	z(void) { return (*this)[0][2] / (*this)[0][3]; }
		void	z(real Z) { (*this)[0][2] = (real)((*this)[0][3] * Z); }
		real	w(void) { return (*this)[0][3]; }
		void	w(real);
	};

	class mLine : public mTransformable
	{
	public:

		mLine(mPoint& point1, mPoint& point2, real par1 = 0, real par2 = 1);
		mLine(void);
		mLine(mLine&);

		mPoint& point(real par);
	};

}	// namespace dkmrx
#endif //dkmrx_geometry_hpp
