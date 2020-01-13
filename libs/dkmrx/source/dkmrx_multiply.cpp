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

#include <utility>
#include "dkmrx_matrix.hpp"
#include "parallel_processor_base.hpp"
#include "multi_threaded_driver.hpp"

using namespace dkmrx;


class ParallelTransposedMatrixMultiplier : public dk::ParallelProcessorBase {
public:
	ParallelTransposedMatrixMultiplier(const matrix& mrx, const matrix& mrxTransposed, matrix& mrxProduct);
	virtual ~ParallelTransposedMatrixMultiplier() override;

	virtual void operator()(size_t inxBegin, size_t inxEnd) override;
	virtual size_t size() override;

private:
	const matrix& mrx_;
	const matrix& mrxTransposed_;
	matrix& mrxProduct_;
};

ParallelTransposedMatrixMultiplier::ParallelTransposedMatrixMultiplier(const matrix& mrx, const matrix& mrxTransposed, matrix& mrxProduct)
	: mrx_{ mrx }, mrxTransposed_{ mrxTransposed }, mrxProduct_{ mrxProduct }
{
}
ParallelTransposedMatrixMultiplier::~ParallelTransposedMatrixMultiplier()
{
}
void ParallelTransposedMatrixMultiplier::operator()(size_t inxBegin, size_t inxEnd)
{
	size_t iMrxTransRows = mrxTransposed_.rows();
	size_t iMrxCols = mrx_.columns();

	for (size_t iRowFirst = inxBegin; iRowFirst < inxEnd; iRowFirst++)
		for (size_t iRowSecond = 0; iRowSecond < iMrxTransRows; iRowSecond++)
		{
			real sum = 0.0;
			for (size_t inx = 0; inx < iMrxCols; inx++)
				sum += mrx_[iRowFirst][inx] * mrxTransposed_[iRowSecond][inx];
			mrxProduct_[iRowFirst][iRowSecond] = sum;
		}
}
size_t ParallelTransposedMatrixMultiplier::size()
{
	return mrx_.rows();
}

void matrix::multiply_MT(const matrix& mrx1, const matrix& mrx2, dk::MultiThreadedDriver* pDriverMT)
{
	bool bCompatible = (mrx1.iColumns_ == mrx2.iRows_);
	_validate(mrx1.pValues_, mrx2.pValues_, bCompatible, "matrix::multiply_MT(const matrix&, const matrix&, dk::MultiThreadedDriver*)");

	// We'll multiply mrx1 by a transposed matrix of mrx2
	// to get better performance out of the CPU cache.
	matrix tempTransposed;
	tempTransposed.transposedOf(mrx2);

	matrix mrxProduct(mrx1.iRows_, mrx2.iColumns_);

	ParallelTransposedMatrixMultiplier mm(mrx1, tempTransposed, mrxProduct);
	pDriverMT->drive(&mm);

	(*this) = std::move(mrxProduct);
}

matrix matrix::operator * (const matrix& mrx) const
{
	bool bCompatible = (iColumns_ == mrx.iRows_);
	_validate(pValues_, mrx.pValues_, bCompatible, "matrix::operator * (const matrix&) const");
	
	matrix mrxProduct(iRows_, mrx.iColumns_);

	for (size_t iRowFirst = 0; iRowFirst < this->iRows_; iRowFirst++)
		for (size_t iColSecond = 0; iColSecond < mrx.iColumns_; iColSecond++)
		{
			real sum = 0.0;
			for (size_t inx = 0; inx < this->iColumns_; inx++)
				sum += (*this)[iRowFirst][inx] * mrx[inx][iColSecond];
			mrxProduct[iRowFirst][iColSecond] = sum;
		}

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

	matrix mrxResult = (*this) * mrx;
	(*this) = std::move(mrxResult);
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
