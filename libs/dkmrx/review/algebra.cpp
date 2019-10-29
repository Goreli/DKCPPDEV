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

