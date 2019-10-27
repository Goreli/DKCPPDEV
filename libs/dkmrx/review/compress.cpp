/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   13 Oct 2001 2:01:21.25p

This media contains programs and data which are proprietary
to David Krikheli.

These contents are provided under a David Krikheli software source
license, which prohibits their unauthorised resale or distribution
outside of the buyer's organization.

THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "matrix.hpp"
#include "merror.hpp"
#include <float.h>
#include <stdlib.h>

///////////// Attention: this code has only been tested for float.
/*
Bit 31 (the bit that is selected by the mask 0x80000000) represents the sign of the floating-point number.
Bits 30-23 (the bits that are selected by the mask 0x7f800000) represent the exponent. 
Bits 22-0 (the bits that are selected by the mask 0x007fffff) represent the significand (sometimes called the mantissa) of the floating-point number. 
If the argument is positive infinity, the result is 0x7f800000. 
If the argument is negative infinity, the result is 0xff800000. 
If the argument is NaN, the result is 0x7fc00000. 
*/
/*
unsigned long getBits(unsigned long value, int startBit, int numBits)
{
	// Get the required bits to position 0.
	value = value >> startBit;

	// Create an appropriate mask.
unsigned long mask = 0L;
	for(int inxBit = 0; inxBit < numBits; inxBit++)
	{
		mask = mask << 1;
		mask += 1L;
	}
		
	return value&mask;
}
void setBits(unsigned long* valuePointer, int startBit, unsigned long bits, int numBits)
{
	// Create an appropriate mask.
unsigned long mask = 0L;
	for(int inxBit = 0; inxBit < numBits; inxBit++)
	{
		mask = mask << 1;
		mask += 1L;
	}

unsigned long invertedMask = ~mask;

	*valuePointer = ((invertedMask<<startBit) & *valuePointer) | ((mask & bits)<<startBit);
const int numBitsInLong = 8*sizeof(unsigned long);
	if(startBit+numBits > numBitsInLong)
		*(valuePointer+1) = ((invertedMask>>(numBitsInLong-startBit)) & *(valuePointer+1)) | ((mask & bits)>>(numBitsInLong-startBit));
}
int getExponentFloat(float value)
{
const int numBitsInLong = 8*sizeof(unsigned long);
const long correction = (long)0x7F;	// 0x3FF for double precision
	return getBits(*((unsigned long*) &value), FLT_MANT_DIG-1, numBitsInLong-FLT_MANT_DIG) - correction;
}

float getRoundedFloat(float value, int quantExp)
{
float returnValue = value;

int numBits = FLT_MANT_DIG-(getExponentFloat(value)-quantExp);
	setBits((unsigned long*)&returnValue, 0, 0x0000L, __min(numBits, FLT_MANT_DIG-1));
	return returnValue;
}
/////////////////// End of Attention.
*/


// Reduces the number of non-zero bits in the IEEE significand.
void matrix::shortenSignificand(real precision, real min, real max)
{
  mError::set();
  if( Values == NULL )
  {
    mError::set( MERR_WRONG_THIS_OBJECT );
    mError::message("Matrix has no values","matrix::shortenSignificand");
    return;
  }

real range = max-min;

register real *elem = Values;
register real *top = Values + Rows*Columns;
  for( ; elem < top; elem++ )
  {
	(*elem) = (*elem);
  }

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::normalise");
    delete this;
  }
}

void matrix::shortenSignificand(real precision)
{
  mError::set();
  if( Values == NULL )
  {
    mError::set( MERR_WRONG_THIS_OBJECT );
    mError::message("Matrix has no values","matrix::shortenSignificand");
    return;
  }

real min;
real max;
  minmax(&min, &max);
  shortenSignificand(precision, min, max);

  if ( Status == TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Applied to a TEMPORARY object","matrix::normalise");
    delete this;
  }
}
