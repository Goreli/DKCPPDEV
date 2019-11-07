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

#ifndef libs_dkmrx_geometry_hpp
#define libs_dkmrx_geometry_hpp

#include "dkmrx_transfrm.hpp"

namespace dkmrx {

	class mPoint : public mTransformable
	{
	public:

		mPoint(real x = 0, real y = 0, real z = 0, real w = 1);
		mPoint(const mPoint&);

		real	distance(const mPoint&) const;
		real	x(void) const { return (*this)[0][0] / (*this)[0][3]; }
		void	x(real X) { (*this)[0][0] = (real)((*this)[0][3] * X); }
		real	y(void) const { return (*this)[0][1] / (*this)[0][3]; }
		void	y(real Y) { (*this)[0][1] = (real)((*this)[0][3] * Y); }
		real	z(void) const { return (*this)[0][2] / (*this)[0][3]; }
		void	z(real Z) { (*this)[0][2] = (real)((*this)[0][3] * Z); }
		real	w(void) const { return (*this)[0][3]; }
		void	w(real);
	};

	class mLine : public mTransformable
	{
	public:

		mLine(const mPoint& point1, const mPoint& point2, real par1 = 0, real par2 = 1);
		mLine(void);
		mLine(const mLine&);

		mPoint point(real par) const;
	};

}	// namespace dkmrx
#endif	 // libs_dkmrx_geometry_hpp
