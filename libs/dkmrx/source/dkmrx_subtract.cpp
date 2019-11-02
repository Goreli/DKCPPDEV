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

#include "dkmrx_matrix.hpp"
#include "dkmrx_error.hpp"

using namespace dkmrx;

matrix matrix::operator - (const matrix& mx) const
{
	mError::set();
	if( (Rows != mx.Rows) ||
	    (Columns != mx.Columns) ||
	    (Values== nullptr)      ||
	    (mx.Values== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator - (const matrix&) const");
		return matrix();
	}

	size_t MatSize=Rows*Columns;

	matrix mrx(Rows, Columns);
	if(mrx.Values == nullptr)
	{
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory", "matrix::operator - (const matrix&) const");
	}
	else 
	{    
	 real *index1,*index2,*index3,*top;
	 
		index1 = top = Values;
		index2 = mx.Values;
		index3 = mrx.Values;
		top   += MatSize;
		for (; index1<top; index1++,index2++,index3++)
		while( index1<top ) *index3++ = *index1++ - *index2++;
	}
	return mrx;
}

matrix matrix::operator - (double k) const
{
	mError::set();

	if( Values == nullptr)
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Matrix has no values","matrix::operator - (double) const");
		return matrix();
	}     

real *start = Values;
real *finish = start + Columns*Rows;

	matrix mrx(Rows, Columns);
	if(mrx.Values == nullptr)
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator - (double) const");
	}
	else
	{
	 	real *dest = mrx.Values;
		while( start < finish )
			*(dest++) = *(start++) - (real)k;
	}

    return mrx;
}

