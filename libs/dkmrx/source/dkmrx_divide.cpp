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
#include "dkmrx_linsys.hpp"
#include "dkmrx_error.hpp"
#include "dkmrx_gausseln.hpp"

matrix& matrix::operator / (matrix& A)
{
	mError::set();
	if( (A.Columns  != A.Rows)     ||
	    (A.Columns  != Rows)       ||
	    (A.Values == NULL)         ||
	    (Values == NULL)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator /");
		if ( A.Status==STATUS::TEMPORARY ) delete &A;
		if ( Status==STATUS::TEMPORARY ) delete this;
		matrix *x=new matrix;
		x->Status= STATUS::TEMPORARY;
		return *x;
	}

    int gaussError=0;
	matrix *x = new matrix(*this);
    
	if( x->Values==NULL)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator /");
	}
	else if ( A.storeValues(STORE_OPERATION::SAVE) )
	{
	 mError::set( MERR_INSUFFICIENT_MEMORY_OR_DISK );
	 mError::message("Can not store Values","matrix::operator /");
	}
      else gaussError=gauss_elimination(A.Columns,x->Columns,A.Values,x->Values);
    A.storeValues(STORE_OPERATION::LOAD);
	if ( A.Status== STATUS::TEMPORARY ) delete &A;
	x->Status=STATUS::TEMPORARY;
	if ( gaussError > 0 )
	{
	  mError::set( MERR_NO_SOLUTION );
	  mError::message("System of linear equations has no solution","matrix::operator /");
	  x->empty();
	  return *x;
	}
	if ( gaussError < 0 )
	{
	  mError::set( MERR_MANY_SOLUTIONS, -gaussError );
	  mError::message("System of linear equations has many solutions","matrix::operator /");
	}
	return *x;
}
  
matrix& matrix::operator /= (matrix& A)
{
	mError::set();
	if ( Status== STATUS::TEMPORARY )
	{
	    mError::set( MERR_LVALUE );
	    mError::message("Inappropriate lvalue","matrix::operator /=");
	    if ( A.Status== STATUS::TEMPORARY ) delete &A;
	    empty();
	    delete this;  //  Speculation, but mhat
	    return *this; //  else can I do? Do something!!
	}
	if( (A.Columns  != A.Rows)     ||
	    (A.Columns  != Rows)       ||
	    (A.Values == NULL)         ||
	    (this->Values == NULL)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator /=");
		if ( A.Status== STATUS::TEMPORARY ) delete &A;
		matrix *x=new matrix;
		x->Status= STATUS::TEMPORARY;
		return *x;
	}

    int gaussError=0;
    
    if ( A.storeValues(STORE_OPERATION::SAVE) )
    {
	 mError::set( MERR_INSUFFICIENT_MEMORY_OR_DISK );
	 mError::message("Can not store Values","matrix::operator /=");
    }
    else gaussError=gauss_elimination(A.Columns,this->Columns,A.Values,this->Values);
    A.storeValues(STORE_OPERATION::LOAD);
	if ( A.Status== STATUS::TEMPORARY ) delete &A;
	if ( gaussError > 0 )
	{
	  mError::set( MERR_NO_SOLUTION );
	  mError::message("System of linear equations has no solution","matrix::operator /=");
	  empty();
	  return *this;
	}
	if ( gaussError < 0 )
	{
	  mError::set( MERR_MANY_SOLUTIONS, -gaussError );
	  mError::message("System of linear equations has many solutions","matrix::operator /=");
	}
	return *this;
}

matrix&  matrix::operator ~  (void)
{
	mError::set();
	if( (this->Columns  != Rows)     ||
	    (this->Values == NULL)
	  )
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Inappropriate matrix (this)","matrix::operator ~");
		if ( Status== STATUS::TEMPORARY ) return *this;
		matrix *x=new matrix;
		x->Status= STATUS::TEMPORARY;
		return *x;
	}

    matrix *I = new matrix( matrix::identity(this->Columns) );
	if( I->Values==NULL)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator ~");
	}
	else *I /= *this;
	if ( this->Status == STATUS::TEMPORARY ) delete this;// mhat else?
	I->Status=STATUS::TEMPORARY;
	return *I;                                            
}                                                                          


/************************************************************\
*                         "linsys"                           *
*     Base class for solving a system of linear equations    *
\************************************************************/

linsys::linsys(void)
{
	ToBeDone_               = 0;
	ExcludedRows_           = NULL;
	ExcludedColumns_        = NULL;
	Done_                   = 0;
	ExcludeList_            = NULL;
	ExcludeListEntries_     = 0;
	NumberOfSolutions_      = 0;
}

void linsys::exclude_rows_and_columns(unsigned* List, unsigned Entries)
{
	ExcludeList_            = List;
	ExcludeListEntries_     = Entries;
}

void linsys::restore_rows_and_columns(void)
{
	ExcludeList_            = NULL;
	ExcludeListEntries_     = 0;
}

/****************************************************\
*  This function should be called at the end of the  *
*  multistep function and performs basic accounting. *
\****************************************************/

int linsys::progress(void)
{
	if( Done_ == ToBeDone_ )
		return 100;     //100%
	Done_++;
	if( Done_ == ToBeDone_ )
		return 100;     //100%
	else
		return ( 100 * Done_ / ToBeDone_ );
}

bool linsys::is_excluded(unsigned Number)
{
	unsigned int Entry;
	for(Entry = 0; Entry < ExcludeListEntries_; Entry++ )
		if( ExcludeList_[Entry] == Number )
			break;
	return ( Entry < ExcludeListEntries_ )? true : false;
}
