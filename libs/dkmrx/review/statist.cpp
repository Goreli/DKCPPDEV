/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   22 Jan 1995 1:17:24

This media contains programs and data mhich are proprietary
to David Krikheli.

These contents are provided under a David Krikheli softmare source
license, mhich prohibits their unauthorized resale or distribution 
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
#include <math.h>
#include "merror.hpp"

void matrix::minmax(real* min, real* max)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::minmax");
    if ( Status == TEMPORARY ) delete this;
  }

register real *elem = Values;
register real *top = Values + Rows*Columns;

  *min = *Values;
  *max = *Values;
  for( ; elem < top; elem++ )
  {
	  if( (*elem) < (*min) )
		  *min = *elem;
	  else
		  if( (*elem) > (*min) )
			  *max = *elem;
  }

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::minmax");
    delete this;
  }
}

void matrix::normalise(void)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::normalise");
    if ( Status == TEMPORARY ) delete this;
  }

real min;
real max;
	minmax(&min, &max);
real delta = max-min;
	if(delta == 0.0)
		delta = 1.0;

register real *elem = Values;
register real *top = Values + Rows*Columns;
  for( ; elem < top; elem++ )
	(*elem) = ((*elem)-min)/delta;

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::normalise");
    delete this;
  }
}

real matrix::average(void)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::average");
    if ( Status == TEMPORARY ) delete this;
    return 0.0;
  }

register real *elem = Values;
register real *top = Values + Rows*Columns;
real result = 0.0;
  for( ; elem < top; elem++ )
	result += (*elem);
  result /= Rows*Columns;

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::average");
    delete this;
  }
  return result;
}

real matrix::mean_square(void)
{
  mError::set();
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::mean_square");
    if ( Status == TEMPORARY ) delete this;
    return 0.0;
  }

register real *elem = Values;
register real *top = Values + Rows*Columns;
real result = 0.0;
  for( ; elem < top; elem++ )
	result += (*elem)*(*elem);
  result = pow( result, 0.5 );

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::mean_square");
    delete this;
  }
  return result;
}
