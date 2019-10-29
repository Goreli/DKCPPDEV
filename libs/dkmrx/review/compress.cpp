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
