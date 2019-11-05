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

#include <stdexcept>
#include <memory>
#include "dkmrx_matrix.hpp"

using namespace dkmrx;

matrix  matrix::operator ~ ()
{
	bool bCompatible = (iColumns_ == iRows_);
	_validate(pValues_, true, bCompatible, "matrix::operator ~ ()");

    matrix mrx = std::move( matrix::identity(iColumns_) );
	mrx /= *this;

	return mrx;
}                                                                          

matrix matrix::identity(int Dim)
{
	if (Dim <= 0)
		throw std::logic_error("Non-positive dimension in static matrix::identity(int)");

	matrix identityMatrix(Dim, Dim, 0.0);
	int   step = Dim + 1;
	real* pTop = identityMatrix.pValues_ + Dim * Dim;
	for (real* pDst = identityMatrix.pValues_; pDst < pTop; pDst += step)
		*pDst = 1;
	return identityMatrix;
}
