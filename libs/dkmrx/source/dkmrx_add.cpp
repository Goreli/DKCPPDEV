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

using namespace dkmrx;

matrix matrix::operator + (const matrix& mrx) const
{
	bool bCompatible = (iRows_ == mrx.iRows_) && (iColumns_ == mrx.iColumns_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator + (const matrix&) const");

	size_t iMatSize=iRows_*iColumns_;
	matrix mrxSum(iRows_,iColumns_);

	real* pSrc1 = pValues_;
	real* pSrc2 = mrx.pValues_;
	real* pDst = mrxSum.pValues_;
	real* pTop = pDst + iMatSize;
	while(pDst < pTop)
		*pDst++ = *pSrc1++ + *pSrc2++;

	return mrxSum;
}

matrix matrix::operator + (real k) const
{
	_validate(pValues_, "matrix::operator + (real) const");

	matrix mrxSum(iRows_, iColumns_);

	real* pSrc = pValues_;
	real* pDst = mrxSum.pValues_;
	real* pTop = pDst + iColumns_ * iRows_;

	while(pDst < pTop)
		*(pDst++) = *(pSrc++) + k;

	return mrxSum;
}

matrix& matrix::operator += (const matrix& mrx)
{
	bool bCompatible = (iRows_ == mrx.iRows_) && (iColumns_ == mrx.iColumns_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator += (const matrix&)");

	real* pSrc = mrx.pValues_;
	real* pDst = pValues_;
	real* pTop = pDst + iRows_ * iColumns_;
	while (pDst < pTop)
		*pDst++ += *pSrc++;

	return *this;
}

matrix& matrix::operator += (real k)
{
	_validate(pValues_, "matrix::operator += (real)");

	real* pDst = pValues_;
	real* pTop = pDst + iRows_ * iColumns_;
	while (pDst < pTop)
		*pDst++ += k;

	return *this;
}
