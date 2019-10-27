/*
  General purpose matrix library, Version 1.0
  Copyright (c) 1994 David Krikheli
 
  Permission to use, copy, modify, distribute, and sell this software and 
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notice and this permission notice appear in
  all copies of the software and related documentation, and (ii) the name of
  David Krikheli may not be used in any advertising or publicity relating to
  the software without the specific, prior written permission of David Krikheli.
  
  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 
  IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
  INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
  POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR 
  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef geometry_h
#define geometry_h

#include "dkmrx_transfrm.hpp"

class mPoint: public mTransformable
{
public:

	mPoint(real x=0, real y=0, real z=0, real w=1);
	mPoint( mPoint& );

	real	distance( mPoint& );
	real	x(void)     { return (*this)[0][0]/(*this)[0][3]; }
	void	x(real X) { (*this)[0][0] = (real)((*this)[0][3] * X); }
	real	y(void)     { return (*this)[0][1]/(*this)[0][3]; }
	void	y(real Y) { (*this)[0][1] = (real)((*this)[0][3] * Y); }
	real	z(void)     { return (*this)[0][2]/(*this)[0][3]; }
	void	z(real Z) { (*this)[0][2] = (real)((*this)[0][3] * Z); }
	real	w(void)     { return (*this)[0][3]; }
	void	w(real);
};

class mLine: public mTransformable
{
public:

	mLine(mPoint& point1, mPoint& point2, real par1 = 0, real par2 = 1);
	mLine( void );
	mLine( mLine& );

	mPoint& point( real par );
};

#endif //geometry_h
