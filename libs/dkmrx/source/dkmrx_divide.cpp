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
	if( (A.Columns  != A.Rows)     ||
	    (A.Columns  != Rows)       ||
	    (A.Values == nullptr)         ||
	    (Values == nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator / (const matrix& A) const");
		return matrix();
	}

    int gaussError=0;
	matrix mrx(*this);
    
	if(mrx.Values == nullptr)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator / (const matrix& A) const");
	}
	else if ( A.storeValues(STORE_OPERATION::SAVE) )
	{
	 mError::set( MERR_INSUFFICIENT_MEMORY_OR_DISK );
	 mError::message("Can not store Values","matrix::operator / (const matrix& A) const");
	}
      else gaussError=gauss_elimination(A.Columns,mrx.Columns,A.Values,mrx.Values);

    A.storeValues(STORE_OPERATION::LOAD);

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
  
matrix& matrix::operator /= (matrix& A)
{
	mError::set();
	if( (A.Columns  != A.Rows)     ||
	    (A.Columns  != Rows)       ||
	    (A.Values == nullptr)         ||
	    (this->Values == nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator /= (const matrix& A)");
		return *this;
	}

    int gaussError=0;
    
    if ( A.storeValues(STORE_OPERATION::SAVE) )
    {
	 mError::set( MERR_INSUFFICIENT_MEMORY_OR_DISK );
	 mError::message("Can not store Values","matrix::operator /= (const matrix& A)");
    }
    else gaussError=gauss_elimination(A.Columns,this->Columns,A.Values,this->Values);

    A.storeValues(STORE_OPERATION::LOAD);

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

matrix  matrix::operator ~ (void)
{
	mError::set();
	if( (this->Columns  != Rows)     ||
	    (this->Values == nullptr)
	  )
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Inappropriate matrix (this)","matrix::operator ~ (void) const");
		return matrix();
	}

    matrix mrx = std::move( matrix::identity(this->Columns) );
	if( mrx.Values== nullptr)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator ~ (void) const");
	}
	else
		mrx /= *this;
	return mrx;
}                                                                          

