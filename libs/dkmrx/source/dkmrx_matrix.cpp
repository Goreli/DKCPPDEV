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

#include <cstring>
#include <cstdio>
#include "dkmrx_matrix.hpp"    
#include "dkmrx_error.hpp"

using namespace dkmrx;

#if defined(MATRIX_TEST)
int	matrix::iConstructCount_ = 0;
int	matrix::iDestructCount_ = 0;
int matrix::iControlSum_ = 0;
#endif

matrix::matrix(void)
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif
	iRows_=iColumns_=0;
	pValues_= nullptr;
	pName_= nullptr;
}

matrix::matrix(int rows,int columns)
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif
	mError::set();
	iRows_=rows;  iColumns_=columns;
	pValues_= nullptr;
	pName_= nullptr;

	if ( (iRows_<=0) || (iColumns_<=0) )
	{
		if ( !(iRows_==0 && iColumns_==0) )
		{
			mError::set( MERR_ILLEGAL_DIMENSION );
			mError::message("Illegal dimension","matrix::matrix(int, int)");
		}
		iRows_=iColumns_=0;
	}
	else
	{
		size_t MatSize=iRows_ * iColumns_;
		pValues_= new real[MatSize];
		if ( pValues_ == nullptr)
		{
			iRows_=iColumns_=0;
			mError::set( MERR_INSUFFICIENT_MEMORY );
			mError::message("Not enough memory","matrix::matrix(int, int)");
		}
	}
}

matrix::matrix(int rows, int columns, real init_value)
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif
	mError::set();
	iRows_=rows;  iColumns_=columns;
    pValues_= nullptr;
	pName_= nullptr;

	if ( (iRows_<=0) || (iColumns_<=0) )
	{
		if ( !(iRows_==0 && iColumns_==0) )
		{
			mError::set( MERR_ILLEGAL_DIMENSION );
			mError::message("Illegal dimension","matrix::matrix(real, int, int)");
		}
		iRows_=iColumns_=0;
	}
	else
	{
		size_t MatSize=iRows_ * iColumns_;
		pValues_= new real[MatSize];
		if ( pValues_ == nullptr)
		{
			iRows_=iColumns_=0;
			mError::set( MERR_INSUFFICIENT_MEMORY );
			mError::message("Not enough memory","matrix::matrix(real, int, int)");
		}
		else
		{
		real *index,*top;

			index = top = pValues_;
			top  += MatSize;
			while( index<top ) *index++ = init_value;
		}
	}
}

matrix::matrix(const matrix& m)
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif
	iRows_=0;  iColumns_=0;
	pValues_= nullptr;
	pName_= nullptr;

	*this=m;
}

matrix::matrix(matrix&& m) noexcept
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif
	iRows_ = 0;  iColumns_ = 0;
	pValues_ = nullptr;
	pName_ = nullptr;

	*this = m;
}

matrix::matrix(const std::initializer_list<const std::initializer_list<real>>& list)
	: iRows_{ (int) list.size() }, iColumns_{ (int) list.begin()->size() }
{
#if defined(MATRIX_TEST)
	iConstructCount_++;
	iSeqId_ = iConstructCount_;
	iControlSum_ += iSeqId_;
#endif

	if ((iRows_ <= 0) || (iColumns_ <= 0))
	{
		if (!(iRows_ == 0 && iColumns_ == 0))
		{
			mError::set(MERR_ILLEGAL_DIMENSION);
			mError::message("Illegal dimension", "matrix::matrix(const std::initializer_list<const std::initializer_list<real>>&)");
		}
		iRows_ = iColumns_ = 0;
	}
	else
	{
		size_t MatSize = iRows_ * iColumns_;
		pValues_ = new real[MatSize];
		if (pValues_ == nullptr)
		{
			iRows_ = iColumns_ = 0;
			mError::set(MERR_INSUFFICIENT_MEMORY);
			mError::message("Not enough memory", "matrix::matrix(const std::initializer_list<const std::initializer_list<real>>&)");
		}
		else
		{
			// Copy the input data.
			size_t inxRow {0};
			for (const auto& inputRow : list)
			{
				if (inputRow.size() != iColumns_)
				{
					mError::set(MERR_ILLEGAL_DIMENSION);
					mError::message("Invalid input row size", "matrix::matrix(const std::initializer_list<const std::initializer_list<real>>&)");
					iRows_ = iColumns_ = 0;
					delete [] pValues_;
					pValues_ = nullptr;
					break;
				}

				size_t inxColumn {0};
				for (const auto& realValue : inputRow)
				{
					(*this)[inxRow][inxColumn] = realValue;
					++inxColumn;
				}
				++inxRow;
			}
		}
	}
}

matrix::~matrix()
{
#if defined(MATRIX_TEST)
	iDestructCount_++;
	iControlSum_ -= iSeqId_;
#endif
	if (pValues_ != nullptr) delete [] pValues_;
	if (pName_   != nullptr) delete [] pName_;
}

void matrix::empty()
{
	iRows_=iColumns_=0;
	if ( pValues_ != nullptr)
	{
		delete [] pValues_;
		pValues_= nullptr;
	}
}

void matrix::name(const char* N)
{                     
  mError::set();
  delete [] pName_;
  if ( N == nullptr) pName_ = nullptr;
  else
  {
    pName_ = new char [ strlen(N) + 1 ];
    if ( pName_ == nullptr)
    {
	mError::set( MERR_INSUFFICIENT_MEMORY );
	mError::message("Not enough memory","matrix::setName");
    }
    else strcpy(pName_,N);
  }
}
