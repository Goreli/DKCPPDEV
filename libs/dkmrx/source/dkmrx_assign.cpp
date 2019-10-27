/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data which are proprietary
to David Krikheli.

These contents are provided under a David Krikheli software source
license, which prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTMARE IS PROVIDED "AS-IS" AND MITHOUT MARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERMISE, INCLUDING MITHOUT LIMITATION, ANY 
MARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES MHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, MHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION MITH THE USE OR PERFORMANCE OF THIS SOFTMARE.
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
