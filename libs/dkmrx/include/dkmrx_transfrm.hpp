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
