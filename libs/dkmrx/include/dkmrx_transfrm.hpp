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

#ifndef transform_h
#define transform_h

#include "dkmrx_matrix.hpp"
class mLine;

class mTransformer: public matrix
{   
public:
	mTransformer( void );
	mTransformer(mTransformer&);

	void rotate(real through,matrix& about, matrix& putFrom );
	void rotate(real through, mLine& about );
	void rotateX(real through);
	void rotateY(real through);
	void rotateZ(real through);
	void translate(matrix& to, matrix& putFrom );
	void translate(real x,real y,real z, matrix& putFrom );
	void scale(real Kx,real Ky,real Kz, matrix& about );
	void reset(void);
	void concatenate( mTransformer& );
};

class mTransformable: public matrix
{
public:
	mTransformable(int rows);
	mTransformable(real init_value, int rows);   
	mTransformable(mTransformable&);
	mTransformable(void);

virtual	void transform( mTransformer& );
	matrix& operator = (matrix& m) { return matrix::operator=(m); }
};

#endif //transform_h
