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
#include <stdlib.h>

using namespace dkmrx;

matrix& matrix::operator + (matrix& mx)
{
	mError::set();
	if( (Rows != mx.Rows) ||
	    (Columns != mx.Columns) ||
	    (Values== NULL)      ||
	    (mx.Values== NULL)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator +");
		if ( Status== STATUS::TEMPORARY ) delete this;
		if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
		matrix *mx=new matrix;
		mx->Status= STATUS::TEMPORARY;
		return *mx;
	}

	size_t MatSize=Rows*Columns;

	if ( Status== STATUS::TEMPORARY )
	{    
		real *index1,*index2,*top;
	 
		index1 = top = Values;
		index2 = mx.Values;
		top   += MatSize;
		while( index1<top ) *index1++ += *index2++;
		if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
		return *this;
	}

	if ( mx.Status== STATUS::TEMPORARY )
	{       
		real *index1,*index2,*top;
	 
		index1 = top = Values;
		index2 = mx.Values;
		top   += MatSize;
		while( index1<top ) *index2++ += *index1++;
		return mx;
	}

	matrix *mx1=new matrix(Rows,Columns);
	if( mx1->Values == NULL )
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator +");
	}
	else 
	{    
		real *index1,*index2,*index3,*top;
	 
		index1 = top = Values;
		index2 = mx.Values;
		index3 = mx1->Values;
		top   += MatSize;
		while( index1<top ) *index3++ = *index1++ + *index2++;
	}
	mx1->Status= STATUS::TEMPORARY;
	return *mx1;
}

matrix& matrix::operator + (double k)
{
	mError::set();
matrix *mx1;

	if( Values == NULL )
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Matrix has no values","matrix::operator +(double)");
		if ( Status == STATUS::TEMPORARY ) return *this;
		mx1=new matrix;
		mx1->Status= STATUS::TEMPORARY;
		return *mx1;
	}             
	real *start = Values;
	real *finish = start + Columns*Rows;
	if( Status == STATUS::TEMPORARY )
	{
		while( start < finish )
			*(start++) += (real)k;
		return *this;
	}
	else
	{
		mx1 = new matrix(Rows, Columns);
	 	if( mx1->Values==NULL)
		{
		  mError::set( MERR_INSUFFICIENT_MEMORY );
		  mError::message("Not enough memory","matrix::operator +(double)");
		}
	 	else
	 	{
	 		real *dest = mx1->Values;
			while( start < finish )
				*(dest++) = *(start++) + (real)k;
		}
	}       
	mx1->Status = STATUS::TEMPORARY;
    return *mx1;
}
