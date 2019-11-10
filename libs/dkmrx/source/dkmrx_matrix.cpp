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
#include <stdexcept>
#include <string>
#include "dkmrx_matrix.hpp"    

using namespace dkmrx;

matrix::matrix(void) noexcept
	: pValues_{ nullptr }, iRows_{ 0 }, iColumns_{ 0 }
{}

matrix::matrix(size_t rows, size_t columns)
	: pValues_{ nullptr }, iRows_{ rows }, iColumns_{ columns }
{
	if ((iRows_ > 0) && (iColumns_ > 0))
		pValues_ = new real[iRows_ * iColumns_];
	else
	{
		iRows_ = iColumns_ = 0;
		throw std::out_of_range("Dimension(s) out of range in matrix::matrix(int, int)");
	}
}

matrix::matrix(size_t rows, size_t columns, real initVal)
	: pValues_{ nullptr }, iRows_{ rows }, iColumns_{ columns }
{
	if ((iRows_ > 0) && (iColumns_ > 0))
	{
		size_t sizeMrx = iRows_ * iColumns_;
		pValues_ = new real[sizeMrx];

		real* pDst = pValues_;
		real* pTop = pDst + sizeMrx;
		while (pDst < pTop)
			*pDst++ = initVal;
	}
	else
	{
		iRows_ = iColumns_ = 0;
		throw std::out_of_range("Dimension(s) out of range in matrix::matrix(int, int, real)");
	}
}

matrix::matrix(const matrix& m)
	: pValues_{ nullptr }, iRows_{ 0 }, iColumns_{ 0 }
{
	*this=m;
}

matrix::matrix(matrix&& m) noexcept
	: pValues_{ nullptr }, iRows_{ 0 }, iColumns_{ 0 }
{
	*this = std::move(m);
}

matrix::matrix(const std::initializer_list<const std::initializer_list<real>>& list)
	: pValues_{nullptr}, iRows_ {(size_t)list.size()}, iColumns_{(size_t)list.begin()->size()}
{
	if ((iRows_ > 0) && (iColumns_ > 0))
	{
		size_t sizeMrx = iRows_ * iColumns_;
		pValues_ = new real[sizeMrx];

		// Copy the input data.
		size_t inxRow{ 0 };
		for (const auto& inputRow : list)
		{
			if (inputRow.size() != iColumns_)
			{
				iRows_ = iColumns_ = 0;
				delete[] pValues_;
				pValues_ = nullptr;
			const char* sFunctionSignature = "matrix::matrix(const std::initializer_list<const std::initializer_list<real>>&)";
				throw std::out_of_range(std::string("Inconsistent lengths of initialiser list entries in ") + sFunctionSignature);
			}

			std::copy(inputRow.begin(), inputRow.end(), (*this)[inxRow]);
			inxRow++;
		}
	}
	else
		iRows_ = iColumns_ = 0;
}

matrix::matrix(size_t rows, const std::initializer_list<real>& list)
	: pValues_{ nullptr }, iRows_{ rows }, iColumns_{ list.size() / rows }
{
	size_t sizeMrx = iRows_ * iColumns_;

	if ((iRows_ > 0) && (iColumns_ > 0) && (list.size() == sizeMrx))
	{
		pValues_ = new real[sizeMrx];
		// Copy the input data.
		std::copy(list.begin(), list.end(), pValues_);
	}
	else
	{
		iRows_ = iColumns_ = 0;
		throw std::out_of_range("Dimension(s) out of range in matrix::matrix(size_t rows, const std::initializer_list<real>& list)");
	}
}



matrix::~matrix()
{
	if (pValues_ != nullptr)
		delete [] pValues_;
}
void matrix::empty() noexcept
{
	iRows_=iColumns_=0;
	if ( pValues_ != nullptr)
	{
		delete [] pValues_;
		pValues_= nullptr;
	}
}
void matrix::_validate(bool bFirstAllocated, bool bSecondAllocated, bool bCompatible, const char* sFunctionSignature) {
	if (!bFirstAllocated)
		throw std::invalid_argument(std::string("First matrix empty in ") + sFunctionSignature);
	else if (!bSecondAllocated)
		throw std::invalid_argument(std::string("Second matrix empty in ") + sFunctionSignature);
	else if (!bCompatible)
		throw std::logic_error(std::string("Incompatible matrices in ") + sFunctionSignature);
}
void matrix::_validate(bool bAllocated, const char* sFunctionSignature) {
	if (!bAllocated)
		throw std::invalid_argument(std::string("Matrix empty in ") + sFunctionSignature);
}
