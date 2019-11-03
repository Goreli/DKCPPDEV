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

using namespace dkmrx;


matrix matrix::operator * (const matrix& mx) const
{
	mError::set();
	if( (iColumns_ != mx.iRows_) ||
	    (pValues_== nullptr)      ||
	    (mx.pValues_== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator * (const matrix& mx) const");
		return matrix();
	}

	matrix mrx(iRows_,mx.iColumns_);
	if( mrx.pValues_== nullptr)
	{
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator * (const matrix& mx) const");
	}
	else 
// ***************** Start of the actual multiplication
{
real* s2;
real  Sum;
real* s1;
real* d;
int   columns;
real* s2Top;
real* firstS2;
real *firstD, *mx1_values, *firstDTop, *dTop;

columns    = mx.iColumns_;
firstD     = mrx.pValues_;
firstDTop  = mrx.pValues_ +   mx.iColumns_;
dTop       = mrx.pValues_ + ( mx.iColumns_ * mrx.iRows_ );
firstS2    = mx.pValues_;
s2Top      = mx.pValues_  + ( mx.iColumns_ * mx.iRows_ );
s1         = pValues_;
mx1_values = pValues_;

	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			s2 = firstS2;  
			Sum = (real) 0.0;
			while ( s2 < s2Top )
			{
				Sum += *s1++ * *s2;
				s2  += columns;
			}
			*d  = Sum;
			 d += columns;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
}
// ***************** End of the actual multiplication

	return mrx;
}

matrix matrix::operator * (real k) const
{
	mError::set();

	if (pValues_ == nullptr)
	{
		mError::set(MERR_WRONG_THIS_OBJECT);
		mError::message("Matrix has no values", "matrix::operator * (real) const");
		return matrix();
	}

	matrix mrx(iRows_, iColumns_);
	if (mrx.pValues_ == nullptr)
	{
		mError::set(MERR_INSUFFICIENT_MEMORY);
		mError::message("Not enough memory", "matrix::operator * (real) const");
	}
	else
	{
		real* pThis = pValues_;
		real* pTop = pThis + iColumns_ * iRows_;
		real* pThat = mrx.pValues_;
		while (pThis < pTop)
			*(pThat++) = *(pThis++) * k;
	}
	return mrx;
}


matrix& matrix::operator *= (const matrix& mx)
{
	mError::set();
	if( (this->iColumns_ != mx.iRows_)  ||
		(this->pValues_== nullptr)       ||
		(mx.pValues_== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator *= (const matrix&)");
		return *this;
	}

	real *temp = new real[(this->iRows_)*(mx.iColumns_)];
	if( temp == nullptr)
	{
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator *= (const matrix&)");
	  return *this;
	}
// ***************** Start of the actual multiplication
{
real* s2;
real  Sum;
real* s1;
real* d;
real* firstS2;
real* dTop;
real *firstD, *mx1_values, *firstDTop;

firstD     = temp;
firstDTop  = temp +  mx.iColumns_;
dTop       = temp + ( mx.iColumns_ * this->iRows_ );
firstS2    = mx.pValues_;
s1         = this->pValues_;
mx1_values = this->pValues_;

  if( this->iColumns_==4 && mx.iColumns_==4 )
  // This case is to support a fast multiplication for the geometry.
  {
#if defined(I387_32)
	multiply_in_assembler_on_4_by_4(this->pValues_, mx.pValues_, temp, this->rows());
#else	// I387_32
	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			Sum = *s1++ * *(s2 = firstS2);
			Sum += *s1++ * *(s2  += 4);
			Sum += *s1++ * *(s2  += 4);
			*d  = Sum + *s1++ * *(s2  += 4);
			 d += 4;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
#endif	// I387_32
  }
  else
  {
	int   columns;
	real* s2Top;

	columns    = mx.iColumns_;
	s2Top      = mx.pValues_  + ( mx.iColumns_ * mx.iRows_ );

	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			s2 = firstS2;
			Sum = (real) 0.0;
			while ( s2 < s2Top )
			{
				Sum += *s1++ * *s2;
				s2  += columns;
			}
			*d  = Sum;
			 d += columns;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
  }
}
// ***************** End of the actual multiplication

	this->iColumns_ = mx.iColumns_;
	delete [] this->pValues_;
	this->pValues_ = temp;
	return *this;
}

matrix& matrix::operator *= (real k)
{
	mError::set();
	if (pValues_ == nullptr)
	{
		mError::set(MERR_INCOMPATIBLE_MATRICES);
		mError::message("Matrix has no values", "matrix::operator *= (real)");
		return *this;
	}

	real* pThis = pValues_;
	real* pTop = pThis + iRows_ * iColumns_;
	while (pThis < pTop)
		*pThis++ *= k;

	return *this;
}
