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

matrix& matrix::operator=(matrix& mx)
{
	mError::set();
	if ( Status== STATUS::TEMPORARY )
	{
	    mError::set( MERR_LVALUE );
	    mError::message("Inappropriate lvalue","matrix::operator =");
	    if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	    empty();
	    delete this;  //  Speculation, but mhat
	    return *this; //  else can I do?
	}

	if( mx.Values == NULL )
	{
	    if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	    empty();
	    return *this;
	}

	if ( Values != NULL ) delete [] Values;
	Rows=mx.Rows;
	Columns=mx.Columns;
	if ( mx.Status == STATUS::TEMPORARY )
	{
	    Values=mx.Values;
	    mx.Values = NULL;
	    delete &mx;
	    return *this;
	}
	size_t MatSize = mx.Rows * mx.Columns;
	Values= new real[MatSize];
	if ( Values == NULL )
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator =");
	    Rows=Columns=0;
	    return *this;
	}
	real *index1,*index2,*top;
	index1= Values;
	index2= mx.Values;
	top   = index1 + MatSize;
	while ( index1<top ) *index1++ = *index2++;
	return *this;
}
