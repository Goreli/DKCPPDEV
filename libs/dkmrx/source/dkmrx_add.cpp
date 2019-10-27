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
