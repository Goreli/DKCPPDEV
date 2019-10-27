/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   24 Feb 1995 23:06

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

#include "matrix.hpp"
#include "merror.hpp"

real matrix::det(void)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY matrix","matrix::det()");
    if ( Status == TEMPORARY )
      delete  this;
    return 0;
  }
  if ( Columns != Rows )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 1 );
    mError::message("Can not apply to a non square matrix","matrix::det()");
    if ( Status == TEMPORARY )
      delete  this;
    return 0;
  }
  if( Columns > 3 )
  {
    mError::set( MERR_NOT_IMPLEMENTED );
    mError::message("Not implemented for large matrices","matrix::det()");
    if ( Status == TEMPORARY )
      delete  this;
    return 0;
  }

real  Det;
register real* Val = Values;

  if( Columns == 3 )
	Det = 
		*Val     * *(Val+4) * *(Val+8)  +
		*(Val+1) * *(Val+5) * *(Val+6)  +
		*(Val+2) * *(Val+3) * *(Val+7)  -
		*(Val+2) * *(Val+4) * *(Val+6)  -
		*Val     * *(Val+5) * *(Val+7)  -
		*(Val+1) * *(Val+3) * *(Val+8);
  else if( Columns == 2 )
	Det = *Val * *(Val+3) - *(Val+1) * *(Val+2);
  else if( Columns == 1 )
	Det = *Val;

  if ( Status == TEMPORARY )
    delete  this;
  return Det;
}
real matrix::trace(void)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY matrix","matrix::trace()");
    if ( Status == TEMPORARY )
      delete  this;
    return 0;
  }
  if ( Columns != Rows )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 1 );
    mError::message("Can not apply to a non square matrix","matrix::trace()");
    if ( Status == TEMPORARY )
      delete  this;
    return 0;
  }

register real* Val   = Values;
register real* Top   = Values + Columns * Rows;
register int   Offset  = Columns + 1;
real  Trace = 0.0;

  while( Val < Top )
  {
	Trace += *Val;
	Val   += Offset;
  }

  if ( Status == TEMPORARY )
    delete  this;
  return Trace;
}

