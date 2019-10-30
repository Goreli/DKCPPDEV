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
#include "dkmrx_error.hpp"

using namespace dkmrx;

#if defined(I387_32)
static void multiply_in_assembler_on_4_by_4
(real* ToTransform, real* Transformer, real* Transformed, unsigned long Rows);
// Implementation of the function allows to specify the Transformed pointer equal to
// the ToTransform pointer. In this case a product will be written to the source matrix.
// We can take advantage of this in the *= operator in order not to allocate the temp area.
#endif	// I387_32

matrix& matrix::operator * (double k)
{
	mError::set();
matrix *mx1;

	if( Values == nullptr)
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Matrix has no values","matrix::operator *(double)");
		if ( Status == STATUS::TEMPORARY ) return *this;
		mx1=new matrix;
		mx1->Status= STATUS::TEMPORARY;
		return *mx1;
	}             
real *start = Values;
real *finish = start + Columns*Rows;
	if( Status == STATUS::TEMPORARY )
	{
		while( start < finish )
			*(start++) *= (real)k;
		return *this;
	}
	else
	{
		mx1 = new matrix(Rows, Columns);
	 	if( mx1->Values== nullptr)
		{
		  mError::set( MERR_INSUFFICIENT_MEMORY );
		  mError::message("Not enough memory","matrix::operator *");
		}
	 	else
	 	{
	 	 real *dest = mx1->Values;
			while( start < finish )
				*(dest++) = *(start++) * (real)k;
		}
	}       
	mx1->Status = STATUS::TEMPORARY;
    return *mx1;
}
matrix& matrix::operator * (matrix& mx)
{
	mError::set();
	if( (Columns != mx.Rows) ||
	    (Values== nullptr)      ||
	    (mx.Values== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator *");
		if ( Status== STATUS::TEMPORARY ) delete this;
		if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
		matrix *mx3=new matrix;
		mx3->Status= STATUS::TEMPORARY;
		return *mx3;
	}

	matrix *mx3=new matrix(Rows,mx.Columns);
	if( mx3->Values== nullptr)
	{
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator *");
	}
	else 
// ***************** Start of the actual multiplication
{
real* s2;
real  Sum;
real* s1;
real* d;
int   columns;
real* s2Top;
real* firstS2;
real *firstD, *mx1_values, *firstDTop, *dTop;

columns    = mx.Columns;
firstD     = mx3->Values;
firstDTop  = mx3->Values +   mx.Columns;
dTop       = mx3->Values + ( mx.Columns * mx3->Rows );
firstS2    = mx.Values;
s2Top      = mx.Values  + ( mx.Columns * mx.Rows );
s1         = Values;
mx1_values = Values;

	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			s2 = firstS2;  
			Sum = (real) 0.0;
			while ( s2 < s2Top )
			{
				Sum += *s1++ * *s2;
				s2  += columns;
			}
			*d  = Sum;
			 d += columns;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
}
// ***************** End of the actual multiplication

	if ( Status== STATUS::TEMPORARY ) delete this;
	if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	mx3->Status= STATUS::TEMPORARY;
	return *mx3;
}

matrix& matrix::operator *= (matrix& mx)
{
	mError::set();
	if ( this->Status== STATUS::TEMPORARY )
	{
	    mError::set( MERR_LVALUE );
		mError::message("Inappropriate lvalue","matrix::operator *=");
	    if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	    this->empty();
	    delete this;  //  Speculation, but what
		return *this; //  else can I do?
	}
	if( (this->Columns != mx.Rows)  ||
		(this->Values== nullptr)       ||
		(mx.Values== nullptr)
	  )
	{
		mError::set( MERR_INCOMPATIBLE_MATRICES );
		mError::message("Incompatible matrices","matrix::operator *=");
		if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
		this->empty();
		return *this;
	}

	real *temp = new real[(this->Rows)*(mx.Columns)];
	if( temp == nullptr)
	{
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator *=");
	  if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	  this->empty();
	  return *this;
	}
// ***************** Start of the actual multiplication
{
real* s2;
real  Sum;
real* s1;
real* d;
real* firstS2;
real* dTop;
real *firstD, *mx1_values, *firstDTop;

firstD     = temp;
firstDTop  = temp +  mx.Columns;
dTop       = temp + ( mx.Columns * this->Rows );
firstS2    = mx.Values;
s1         = this->Values;
mx1_values = this->Values;

  if( this->Columns==4 && mx.Columns==4 )
  // This case is to support a fast multiplication for the geometry.
  {
#if defined(I387_32)
	multiply_in_assembler_on_4_by_4(this->Values, mx.Values, temp, this->rows());
#else	// I387_32
	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			Sum = *s1++ * *(s2 = firstS2);
			Sum += *s1++ * *(s2  += 4);
			Sum += *s1++ * *(s2  += 4);
			*d  = Sum + *s1++ * *(s2  += 4);
			 d += 4;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
#endif	// I387_32
  }
  else
  {
	int   columns;
	real* s2Top;

	columns    = mx.Columns;
	s2Top      = mx.Values  + ( mx.Columns * mx.Rows );

	while( firstD < firstDTop )
	{
		d = firstD;
		while ( d < dTop )
		{
			s2 = firstS2;
			Sum = (real) 0.0;
			while ( s2 < s2Top )
			{
				Sum += *s1++ * *s2;
				s2  += columns;
			}
			*d  = Sum;
			 d += columns;
		}
		firstD++;
		firstS2++;
		s1 = mx1_values;
	}
  }
}
// ***************** End of the actual multiplication

	this->Columns = mx.Columns;
	if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	delete [] this->Values;
	this->Values = temp;
	return *this;
}

#if defined(I387_32)
static void multiply_in_assembler_on_4_by_4
(real* ToTransform, real* Transformer, real* Transformed, unsigned long Rows)
{
real*	CoordPtr=ToTransform;			// Source.
real*	TransPtr=Transformer;			// Transformer.
real*	ResultPtr=Transformed;			// Destination.
unsigned long	NumRows=Rows;			// Number of rows in the source array.

__asm
{
	pushad	;// Save registers.

	;// Initialize pointers to the arrays.
	mov	eax, CoordPtr			;// Initialize a pointer to a row of coordinates.
	mov	ebx, TransPtr			;// Initialize a pointer to a column of the Transformer.
	mov	edx, ResultPtr			;// Initialize the result pointer.
	mov	ecx, NumRows			;// Load the row counter.

	;// Load the coprocessor with one row of coordinates.
	;// This will bring us to a state that we will call "INITIAL STATE".
load_coordinates:
	fld	qword ptr (0*8)[eax]	;// Load the first quantity of the row.
	fld	qword ptr (1*8)[eax]
	fld	qword ptr (2*8)[eax]
	fld	qword ptr (3*8)[eax]	;// Load the last quantity of the row.

	;// Load the coprocessor with the first column from the Transformer matrix
	;// and perform a scalar multiplication on the row of coordinates already kept there.
	fld	qword ptr ( 0*8)[ebx]	;// Load the first quantity of the column
	fmul	st, st(4)			;// and multiply it by the first coordinate.
;// Take advantage of the parallel processing and advance a pointer sitting in eax.
add	eax, 4*8					;// Point to the next row of coordinates.
	fld	qword ptr ( 4*8)[ebx]	;// Load the second quantity of the column
	fmul	st, st(4)			;// and multiply it by the second coordinate.
	fld	qword ptr ( 8*8)[ebx]	;// Load the third quantity of the column
	fmul	st, st(4)			;// and multiply it by the third coordinate.
	fld	qword ptr (12*8)[ebx]	;// Load the fourth quantity of the column
	fmul	st, st(4)			;// and multiply it by the fourth coordinate.
	;// Calculate and save the sum of the four products.
	fadd						;// Fourth + Third
	fadd						;// Fourth + Third + Second
	fadd						;// Fourth + Third + Second + First
	fstp	qword ptr ( 0*8)[edx]	;// Record the sum.
	;// At this stage we are back to the INITIAL STATE when the coprocessor is loaded
	;// with four quantities from a row of the Coordinates matrix.

	;// Load the coprocessor with the second column from the Transformer matrix
	;// and perform a scalar multiplication on the row of coordinates already kept there.
	fld	qword ptr ( 1*8)[ebx]	;// Load the first quantity of the column
	fmul	st, st(4)			;// and multiply it by the first coordinate.
	fld	qword ptr ( 5*8)[ebx]	;// Load the second quantity of the column
	fmul	st, st(4)			;// and multiply it by the second coordinate.
	fld	qword ptr ( 9*8)[ebx]	;// Load the third quantity of the column
	fmul	st, st(4)			;// and multiply it by the third coordinate.
	fld	qword ptr (13*8)[ebx]	;// Load the fourth quantity of the column
	fmul	st, st(4)			;// and multiply it by the fourth coordinate.
	;// Calculate and save the sum of the four products.
	fadd						;// Fourth + Third
	fadd						;// Fourth + Third + Second
	fadd						;// Fourth + Third + Second + First
	fstp	qword ptr ( 1*8)[edx]	;// Record the sum.
	;// At this stage we are back to the INITIAL STATE when the coprocessor is loaded
	;// with four quantities from a row of the Coordinates matrix.

	;// Load the coprocessor with the third column from the Transformer matrix
	;// and perform a scalar multiplication on the row of coordinates already kept there.
	fld	qword ptr ( 2*8)[ebx]	;// Load the first quantity of the column
	fmul	st, st(4)			;// and multiply it by the first coordinate.
	fld	qword ptr ( 6*8)[ebx]	;// Load the second quantity of the column
	fmul	st, st(4)			;// and multiply it by the second coordinate.
	fld	qword ptr (10*8)[ebx]	;// Load the third quantity of the column
	fmul	st, st(4)			;// and multiply it by the third coordinate.
	fld	qword ptr (14*8)[ebx]	;// Load the fourth quantity of the column
	fmul	st, st(4)			;// and multiply it by the fourth coordinate.
	;// Calculate and save the sum of the four products.
	fadd						;// Fourth + Third
	fadd						;// Fourth + Third + Second
	fadd						;// Fourth + Third + Second + First
	fstp	qword ptr ( 2*8)[edx]	;// Record the sum.
	;// At this stage we are back to the INITIAL STATE when the coprocessor is loaded
	;// with four quantities from a row of the Coordinates matrix.

	;// Load the coprocessor with the fourth column from the Transformer matrix
	;// and perform a scalar multiplication on the row of coordinates already kept there.
	fld	qword ptr ( 3*8)[ebx]	;// Load the first quantity of the column
	fmul	st, st(4)			;// and multiply it by the first coordinate.
	fld	qword ptr ( 7*8)[ebx]	;// Load the second quantity of the column
	fmul	st, st(4)			;// and multiply it by the second coordinate.
	fld	qword ptr (11*8)[ebx]	;// Load the third quantity of the column
	fmul	st, st(4)			;// and multiply it by the third coordinate.
	fld	qword ptr (15*8)[ebx]	;// Load the fourth quantity of the column
	fmul	st, st(4)			;// and multiply it by the fourth coordinate.
	;// Calculate and save the sum of the four products.
	fadd						;// Fourth + Third
	fadd						;// Fourth + Third + Second
	fadd						;// Fourth + Third + Second + First
	fstp	qword ptr ( 3*8)[edx]	;// Record the sum.
	;// At this stage we are back to the INITIAL STATE when the coprocessor is loaded
	;// with four quantities from a row of the Coordinates matrix.

	;// Flush the coprocessor by poping four initial coordinates from the coprocessor in order
	;// not to generate an exception during the next session.
	fcompp						;// Pop two registers from the coprocessor.
;// Take advantage of the parallel processing and advance a pointer sitting in edx.
add	edx, 4*8					;// Point to the next row of results.
	fcompp						;// Pop two registers from the coprocessor.

	;// Process the next row. CODE FROM THIS BLOCK MUST BE SPREAD INTO THE PREVIOUS
	;// SECTION IN ORDER TO TAKE ADVANTAGE OF THE PARALLEL PROCESSING.
	dec	ecx						;// The loopd statement does not work here unfortunately
	jnz	load_coordinates		;// because it attempts to make a short jump.

	popad						;// Restore registers.
}	// __asm
}
#endif	// I387_32

