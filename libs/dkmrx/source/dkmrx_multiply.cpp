/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#include "dkmrx_matrix.hpp"

using namespace dkmrx;

/*
matrix matrix::operator * (const matrix& mrx) const
{
	bool bCompatible = (iColumns_ == mrx.iRows_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator * (const matrix&) const");

	matrix mrxProduct(iRows_, mrx.iColumns_);
	// ***************** Start of the actual multiplication
	for(size_t iRowFirst = 0; iRowFirst < this->iRows_; iRowFirst++)
		for (size_t iColSecond = 0; iColSecond < mrx.iColumns_; iColSecond++)
		{
			real sum = 0.0;
			for (size_t inx = 0; inx < this->iColumns_; inx++)
				sum += this->operator[](iRowFirst)[inx] * mrx[inx][iColSecond];
			mrxProduct[iRowFirst][iColSecond] = sum;
		}
	// ***************** End of the actual multiplication

	return mrxProduct;
}
*/

matrix matrix::operator * (const matrix& mrx) const
{
	bool bCompatible = (iColumns_ == mrx.iRows_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator * (const matrix&) const");

	matrix mrxProduct(iRows_, mrx.iColumns_);

	// We'll multiply this matrix by a transposed mrx matrix
	// to get better performance out of the CPU cache.
	matrix tempTransposed = mrx;
	tempTransposed.transpose();
	// ***************** Start of the actual multiplication
	for(size_t iRowFirst = 0; iRowFirst < this->iRows_; iRowFirst++)
		for (size_t iRowSecond = 0; iRowSecond < tempTransposed.iRows_; iRowSecond++)
		{
			real sum = 0.0;
			for (size_t inx = 0; inx < this->iColumns_; inx++)
				sum += this->operator[](iRowFirst)[inx] * tempTransposed[iRowSecond][inx];
			mrxProduct[iRowFirst][iRowSecond] = sum;
		}
	// ***************** End of the actual multiplication

	return mrxProduct;
}


matrix matrix::operator * (real k) const
{
	_validate(pValues_, "matrix::operator * (real) const");

	matrix mrxProduct(iRows_, iColumns_);

	real* pSrc = pValues_;
	real* pDst = mrxProduct.pValues_;
	real* pTop = pDst + iColumns_ * iRows_;

	while (pDst < pTop)
		*(pDst++) = *(pSrc++) * k;

	return mrxProduct;
}

matrix& matrix::operator *= (const matrix& mrx)
{
	bool bCompatible = (this->iColumns_ == mrx.iRows_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator *= (const matrix&)");

	real *temp = new real[(this->iRows_)*(mrx.iColumns_)];

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
firstDTop  = temp + mrx.iColumns_;
dTop       = temp + (mrx.iColumns_ * this->iRows_ );
firstS2    = mrx.pValues_;
s1         = this->pValues_;
mx1_values = this->pValues_;

  if( this->iColumns_==4 && mrx.iColumns_==4 )
  // This case is to support a fast multiplication for the geometry.
  {
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
  }
  else
  {
	size_t columns;
	real* s2Top;

	columns    = mrx.iColumns_;
	s2Top      = mrx.pValues_  + (mrx.iColumns_ * mrx.iRows_ );

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

	this->iColumns_ = mrx.iColumns_;
	delete [] this->pValues_;
	this->pValues_ = temp;
	return *this;
}

matrix& matrix::operator *= (real k)
{
	_validate(pValues_, "matrix::operator *= (real)");

	real* pDst = pValues_;
	real* pTop = pDst + iRows_ * iColumns_;
	while (pDst < pTop)
		*pDst++ *= k;

	return *this;
}
