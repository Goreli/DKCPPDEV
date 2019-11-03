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

matrix matrix::identity(int Dim)
{
	mError::set();

	if (Dim <= 0)
	{
		mError::set(MERR_ILLEGAL_DIMENSION);
		mError::message("Illegal dimension", "matrix::identity(int)");
		return matrix();
	}
	else
	{
		matrix identityMatrix(0.0, Dim, Dim);
		if (identityMatrix.pValues_ == nullptr)
		{
			mError::set(MERR_INSUFFICIENT_MEMORY);
			mError::message("Not enough memory", "matrix::identity(int)");
		}
		else
		{
			int   step = Dim + 1;
			real* Ptr;
			real* PtrTop = identityMatrix.pValues_ + Dim * Dim;
			for (Ptr = identityMatrix.pValues_; Ptr < PtrTop; Ptr += step) *Ptr = 1.0;
		}
		return identityMatrix;
	}
}
