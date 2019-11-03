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

matrix matrix::operator + (const matrix& mx) const
{
	mError::set();
	if( (iRows_ != mx.iRows_) ||
	    (iColumns_ != mx.iColumns_) ||
	    (pValues_== nullptr)      ||
	    (mx.pValues_== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator + (const matrix& mx) const");
		return matrix();
	}

	size_t iMatSize=iRows_*iColumns_;

	matrix matrixSum(iRows_,iColumns_);
	if(matrixSum.pValues_ == nullptr)
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator +");
	}
	else 
	{    
		real *index1,*index2,*index3,*top;
	 
		index1 = top = pValues_;
		index2 = mx.pValues_;
		index3 = matrixSum.pValues_;
		top   += iMatSize;
		while( index1<top ) *index3++ = *index1++ + *index2++;
	}
	return matrixSum;
}

matrix matrix::operator + (real k) const
{
	mError::set();
	if( pValues_ == nullptr)
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Matrix has no values","matrix::operator + (real) const");
		return matrix();
	}             

	matrix mrx(iRows_, iColumns_);
	if( mrx.pValues_ == nullptr)
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator + (real) const");
	}
	else
	{
		real* pStart = pValues_;
		real* pFinish = pStart + iColumns_ * iRows_;
		real *pDest = mrx.pValues_;

		while( pStart < pFinish)
			*(pDest++) = *(pStart++) + k;
	}
    return mrx;
}

matrix& matrix::operator += (const matrix& mx)
{
	mError::set();
	if ((iRows_ != mx.iRows_) ||
		(iColumns_ != mx.iColumns_) ||
		(pValues_ == nullptr) ||
		(mx.pValues_ == nullptr)
		)
	{
		mError::set(MERR_INCOMPATIBLE_MATRICES);
		mError::message("Incompatible matrices", "matrix::operator += (const matrix&)");
		return *this;
	}

	real* pThis = pValues_;
	real* pThat = mx.pValues_;
	real* pTop = pThis + iRows_ * iColumns_;
	while (pThis < pTop)
		*pThis++ += *pThat++;

	return *this;
}

matrix& matrix::operator += (real k)
{
	mError::set();
	if (pValues_ == nullptr)
	{
		mError::set(MERR_INCOMPATIBLE_MATRICES);
		mError::message("Matrix has no values", "matrix::operator += (real)");
		return *this;
	}

	real* pThis = pValues_;
	real* pTop = pThis + iRows_ * iColumns_;
	while (pThis < pTop)
		*pThis++ += k;

	return *this;
}
