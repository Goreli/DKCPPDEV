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

#include <memory>
#include "dkmrx_matrix.hpp"
#include "dkmrx_error.hpp"
#include "dkmrx_gausseln.hpp"

using namespace dkmrx;

matrix matrix::operator / (matrix& A) const
{
	mError::set();
	if( (A.iColumns_  != A.iRows_)     ||
	    (A.iColumns_  != iRows_)       ||
	    (A.pValues_ == nullptr)         ||
	    (pValues_ == nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator / (const matrix& A) const");
		return matrix();
	}

    int gaussError=0;
	matrix mrx(*this);
    
	if(mrx.pValues_ == nullptr)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator / (const matrix& A) const");
	}
    else
		gaussError=gauss_elimination(A.iColumns_,mrx.iColumns_,A.pValues_,mrx.pValues_);

	if ( gaussError > 0 )
	{
	  mError::set( MERR_NO_SOLUTION );
	  mError::message("System of linear equations has no solution","matrix::operator / (const matrix& A) const");
	  mrx.empty();
	  return mrx;
	}

	if ( gaussError < 0 )
	{
	  mError::set( MERR_MANY_SOLUTIONS, -gaussError );
	  mError::message("System of linear equations has many solutions","matrix::operator / (const matrix& A) const");
	}
	return mrx;
}

matrix matrix::operator / (real k) const
{
	mError::set();

	if (pValues_ == nullptr)
	{
		mError::set(MERR_WRONG_THIS_OBJECT);
		mError::message("Matrix has no values", "matrix::operator / (real) const");
		return matrix();
	}

	matrix mrx(iRows_, iColumns_);
	if (mrx.pValues_ == nullptr)
	{
		mError::set(MERR_INSUFFICIENT_MEMORY);
		mError::message("Not enough memory", "matrix::operator / (real) const");
	}
	else
	{
		real* pThis = pValues_;
		real* pTop = pThis + iColumns_ * iRows_;
		real* pThat = mrx.pValues_;
		while (pThis < pTop)
			*(pThat++) = *(pThis++) / k;
	}
	return mrx;
}


matrix& matrix::operator /= (matrix& A)
{
	mError::set();
	if( (A.iColumns_  != A.iRows_)     ||
	    (A.iColumns_  != iRows_)       ||
	    (A.pValues_ == nullptr)         ||
	    (this->pValues_ == nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator /= (const matrix& A)");
		return *this;
	}

    int gaussError=0;
    
    gaussError=gauss_elimination(A.iColumns_,this->iColumns_,A.pValues_,this->pValues_);

	if ( gaussError > 0 )
	{
	  mError::set( MERR_NO_SOLUTION );
	  mError::message("System of linear equations has no solution","matrix::operator /= (const matrix& A)");
	  empty();
	  return *this;
	}
	if ( gaussError < 0 )
	{
	  mError::set( MERR_MANY_SOLUTIONS, -gaussError );
	  mError::message("System of linear equations has many solutions","matrix::operator /= (const matrix& A)");
	}
	return *this;
}

matrix& matrix::operator /= (real k)
{
	mError::set();
	if (pValues_ == nullptr)
	{
		mError::set(MERR_INCOMPATIBLE_MATRICES);
		mError::message("Matrix has no values", "matrix::operator /= (real)");
		return *this;
	}

	real* pThis = pValues_;
	real* pTop = pThis + iRows_ * iColumns_;
	while (pThis < pTop)
		*pThis++ /= k;

	return *this;
}

matrix  matrix::operator ~ (void)
{
	mError::set();
	if( (this->iColumns_  != iRows_)     ||
	    (this->pValues_ == nullptr)
	  )
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Inappropriate matrix (this)","matrix::operator ~ (void) const");
		return matrix();
	}

    matrix mrx = std::move( matrix::identity(this->iColumns_) );
	if( mrx.pValues_== nullptr)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator ~ (void) const");
	}
	else
		mrx /= *this;
	return mrx;
}                                                                          

