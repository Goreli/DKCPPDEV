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
#include <utility>

using namespace dkmrx;

void matrix::transposedOf(const matrix& mrx)
{
	_validate(mrx.pValues_, "matrix::transposedOf(const matrix&)");

	if (iColumns_ * iRows_ != mrx.iColumns_ * mrx.iRows_) {
		empty();
		pValues_ = new real[mrx.iColumns_ * mrx.iRows_];
	}
	iColumns_ = mrx.iRows_;
	iRows_ = mrx.iColumns_;

	for (size_t iRow = 0; iRow < iRows_; iRow++)
		for (size_t iCol = 0; iCol < iColumns_; iCol++)
			(*this)[iRow][iCol] = mrx[iCol][iRow];
}
void matrix::transpose()
{
	_validate(pValues_, "matrix::transpose()");

	matrix tempTransposed;
	tempTransposed.transposedOf(*this);

	(*this) = std::move(tempTransposed);
}
