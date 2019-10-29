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
