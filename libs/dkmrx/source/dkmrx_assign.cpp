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

matrix& matrix::operator=(const matrix& mrx)
{
	// Make sure the response to the empty agrument case is consistent with the move assignment operator.
	// The move assignment operator is expected to be noexcept, so we wont throw an exception here either.
	if(mrx.pValues_ == nullptr)
	{
		empty();
		return *this;
	}

	if (pValues_ != nullptr) 
		delete[] pValues_;
	size_t MatSize = mrx.iRows_ * mrx.iColumns_;
	try {
		pValues_ = new real[MatSize];
	}
	catch (...) {
		iRows_ = iColumns_ = 0;
		throw;
	}
	iRows_ = mrx.iRows_;
	iColumns_ = mrx.iColumns_;

	real* pSrc = mrx.pValues_;
	real* pDst = pValues_;
	real* pTop = pDst + MatSize;
	while (pDst < pTop)
		*pDst++ = *pSrc++;
	return *this;
}

matrix& matrix::operator=(matrix&& mrx) noexcept
{
	if(mrx.pValues_ == nullptr)
	{
		empty();
		return *this;
	}

	if (pValues_ != nullptr)
		delete[] pValues_;
	iRows_ = mrx.iRows_;
	iColumns_ = mrx.iColumns_;
	pValues_ = mrx.pValues_;

	mrx.pValues_ = nullptr;
	mrx.iRows_ = 0;
	mrx.iColumns_ = 0;
	return *this;
}
