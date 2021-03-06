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

#include <memory>
#include "dkmrx_gausseln.hpp"
#include "dkmrx_gausselnx.hpp"

using namespace dkmrx;

matrix matrix::operator / (matrix& A) const
{
	bool bCompatible = (A.iColumns_ == A.iRows_) && (A.iColumns_ == iRows_);
	_validate(pValues_, A.pValues_, bCompatible, "matrix::operator / (matrix&) const");

	matrix mrxX(*this);

	size_t iNumSols = gauss_elimination(A.pValues_, A.iColumns_, mrxX.pValues_, mrxX.iColumns_);

	if (iNumSols != 1)
	{
		std::string sMsg = " in matrix::operator / (const matrix&) const";
		if (iNumSols > 1)
			sMsg = "System of linear equations has many solutions" + sMsg;
		else
		{
			sMsg = "System of linear equations has no solution" + sMsg;
			mrxX.empty();
		}

		throw GaussEliminationException(sMsg, iNumSols, std::move(mrxX));
	}

	return mrxX;
}

matrix matrix::operator / (real k) const
{
	_validate(pValues_, "matrix::operator / (real) const");

	matrix mrxDiv(iRows_, iColumns_);

	real* pSrc = pValues_;
	real* pDst = mrxDiv.pValues_;
	real* pTop = pDst + iColumns_ * iRows_;
	while (pDst < pTop)
		*(pDst++) = *(pSrc++) / k;

	return mrxDiv;
}


matrix& matrix::operator /= (matrix& A)
{
	bool bCompatible = (A.iColumns_ == A.iRows_) && (A.iColumns_ == iRows_);
	_validate(pValues_, A.pValues_, bCompatible, "matrix::operator /= (const matrix&)");

	size_t iNumSols = gauss_elimination(A.pValues_, A.iColumns_, this->pValues_, this->iColumns_);

	if (iNumSols != 1)
	{
		std::string sMsg = " in matrix::operator /= (const matrix&)";
		if (iNumSols > 1)
			sMsg = "System of linear equations has many solutions" + sMsg;
		else
		{
			sMsg = "System of linear equations has no solution" + sMsg;
			empty();
		}

		throw GaussEliminationException(sMsg, iNumSols, std::move(*this));
	}

	return *this;
}

matrix& matrix::operator /= (real k)
{
	_validate(pValues_, "matrix::operator /= (real)");

	real* pDst = pValues_;
	real* pTop = pDst + iRows_ * iColumns_;
	while (pDst < pTop)
		*pDst++ /= k;

	return *this;
}

void matrix::makeUpperTriangular()
{
	bool bCompatible = (iColumns_ == iRows_);
	_validate(pValues_, true, bCompatible, "matrix::makeUpperTriangular()");

	make_upper_triangular(pValues_, iColumns_);
}
