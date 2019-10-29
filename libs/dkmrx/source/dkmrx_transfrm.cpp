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

// All coordinates are considered in the world (right) coordinate system.
#include "dkmrx_geometry.hpp"
#include <stdlib.h>
#include <math.h>
#include "dkmrx_error.hpp"


mTransformer::mTransformer(void):matrix(4,4)
{
real* values = (*this)[0];

	*values  =1;	*++values=0;	*++values=0;	*++values=0;
	*++values=0;	*++values=1;	*++values=0;	*++values=0;
	*++values=0;	*++values=0;	*++values=1;	*++values=0;
	*++values=0;	*++values=0;	*++values=0;	*++values=1;
}

mTransformer::mTransformer(mTransformer& kit):matrix(kit) {}

void mTransformer::reset(void)
{
real* values = (*this)[0];

	*values  =1;	*++values=0;	*++values=0;	*++values=0;
	*++values=0;	*++values=1;	*++values=0;	*++values=0;
	*++values=0;	*++values=0;	*++values=1;	*++values=0;
	*++values=0;	*++values=0;	*++values=0;	*++values=1;
}

void mTransformer::rotate(real through,matrix& about,matrix& putFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 3 );
    mError::message("Wrong third argument","mTransformer::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    return;
  }
  if ( about[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 2 );
    mError::message("Wrong second argument","mTransformer::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    return;
  }                          
 
  mTransformer *r = new mTransformer; 
  if( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","mTransformer::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    return;
  } 
  if ( (*r)[0] == NULL  )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","mTransformer::rotation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    delete r;
    return;
  }

#if defined(I387_32)

  real v;
  real *VPtr=&v;

  real *PutFromPtr = putFrom[0];
  real *AboutPtr = about[0];
  real *ThroughPtr = &through;
  real *DestPtr = (*r)[0];

__asm
{
	pushad	;// Save registers.

	// Begin normalization.
	;//real a=about[0][0];   real b=about[0][1];   real c=about[0][2];
	;//real v = sqrt(a*a+b*b+c*c);
	mov	eax, AboutPtr		;// Load a pointer to the about vector.
	fld	qword ptr (2*8)[eax]	;// Load about[0][2].
	fld	qword ptr (1*8)[eax]	;// Load about[0][1].
	fld	qword ptr (0*8)[eax]	;// Load about[0][0].
	fld	st(2)			;// Calculate c*c.
	fmul	st, st(3)
	fld	st(2)			;// Calculate b*b.
	fmul	st, st(3)
	fld	st(2)			;// Calculate a*a.
	fmul	st, st(3)
	fadd				;// Calculate b*b+a*a.
	fadd				;// Calculate v=c*c+b*b+a*a.
	fsqrt				;// Calculate v=sqrt(v).

	;// Save v. It will be used by the C conditional statements.
	mov	eax, VPtr
	fst	qword ptr [eax]
	;// If v==0 do not continue. For this: get the coprocessor condition bits into
	;// the ax and transfer them to the CPU flags register. Then use the
	;// ordinary flow control instructions.
	fxam		;// Report the contents of the st in the status word.
	fstsw	ax	;// Transfer contents of the status word to the ax register.
	shr	ax, 8	;// Get the high byte in the low byte.
	sahf		;// Put ah into the CPU flags register.
	jz	pop_4_and_finish

	;// a/=v; b/=v; c/=v;
	fdiv	st(3),st		;// c/=v;
	fdiv	st(2),st		;// b/=v;
	fdivp	st(1),st		;// a/=v and pop v;
	;// Normalization done. The coprocessor stack looks like this:
	;// a - in st(0)
	;// b - in st(1)
	;// c - in st(2)

	fld1				;// Push one onto the coprocessor stack.
	mov	eax, ThroughPtr		;// Load a pointer to the through angle.
	fld	qword ptr [eax]		;// Load the through angle.
	fsincos				;// cos on top, then sin. Then 1 goes...
	fsubp	st(2), st		;// Z1=1-cos(through)
	;// The coprocessor stack looks like this:
	;// z2 - in st(0)
	;// Z1 - in st(1)
	;// a  - in st(2)
	;// b  - in st(3)
	;// c  - in st(4)

	mov	ecx, DestPtr			;// Load a pointer to the destination memory area.
	fld	st(2)				;// Load a.
	fmul	st, st(4)			;// a*b.
	fmul	st, st(2)			;// a*b*Z1.
	fld	st(5)				;// Load c.
	fmul	st, st(2)			;// c*z2.
	fld	st(1)				;// Load a*b*Z1.
	fsub	st, st(1)			;// a*b*Z1 - c*z2.
	fstp	qword ptr (0*4*8 + 1*8)[ecx]	;// (*r)[0][1] = (real)(ABZ1 - CZ2)
	fld	st(1)				;// Load a*b*Z1.
	fadd	st, st(1)			;// a*b*Z1 + c*z2.
	fstp	qword ptr (1*4*8 + 0*8)[ecx]	;// (*r)[1][0] = (real)(ABZ1 + CZ2)
	fcompp					;// Clean the stack.
	;// The coprocessor stack looks like this:
	;// z2 - in st(0)
	;// Z1 - in st(1)
	;// a  - in st(2)
	;// b  - in st(3)
	;// c  - in st(4)

	fld	st(2)				;// Load a.
	fmul	st, st(5)			;// a*c.
	fmul	st, st(2)			;// a*c*Z1.
	fld	st(4)				;// Load b.
	fmul	st, st(2)			;// b*z2.
	fld	st(1)				;// Load a*c*Z1.
	fadd	st, st(1)			;// a*c*Z1 + b*z2.
	fstp	qword ptr (0*4*8 + 2*8)[ecx]	;// (*r)[0][2] = (real)(ACZ1 + BZ2)
	fld	st(1)				;// Load a*c*Z1.
	fsub	st, st(1)			;// a*c*Z1 - b*z2.
	fstp	qword ptr (2*4*8 + 0*8)[ecx]	;// (*r)[2][0] = (real)(ACZ1 - BZ2)
	fcompp					;// Clean the stack.
	;// The coprocessor stack looks like this:
	;// z2 - in st(0)
	;// Z1 - in st(1)
	;// a  - in st(2)
	;// b  - in st(3)
	;// c  - in st(4)

	fld	st(3)				;// Load b.
	fmul	st, st(5)			;// b*c.
	fmul	st, st(2)			;// b*c*Z1.
	fld	st(3)				;// Load a.
	fmul	st, st(2)			;// a*z2.
	fld	st(1)				;// Load b*c*Z1.
	fsub	st, st(1)			;// b*c*Z1 - a*z2.
	fstp	qword ptr (1*4*8 + 2*8)[ecx]	;// (*r)[1][2] = (real)(BCZ1 - AZ2)
	fld	st(1)				;// Load b*c*Z1.
	fadd	st, st(1)			;// b*c*Z1 + a*z2.
	fstp	qword ptr (2*4*8 + 1*8)[ecx]	;// (*r)[2][1] = (real)(BCZ1 + AZ2)
	fcompp					;// Clean the stack.
	;// The coprocessor stack looks like this:
	;// z2 - in st(0)
	;// Z1 - in st(1)
	;// a  - in st(2)
	;// b  - in st(3)
	;// c  - in st(4)

	fcomp					;// Pop z2.
	fxch					;// Swap Z1 and a.
	fcomp					;// Pop a.
	;// The coprocessor stack looks like this:
	;// Z1 - in st(0)
	;// b  - in st(1)
	;// c  - in st(2)

	fld	st(1)				;// Load b.
	fmulp	st(2), st			;// b_2=b*b;
	fld	st(2)				;// Load c.
	fmulp	st(3), st			;// c_2=c*c;
	fmul	st(1), st			;// Z1b_2
	fmul	st(2), st			;// Z1c_2
	;// The coprocessor stack looks like this:
	;// Z1    - in st(0)
	;// Z1b_2 - in st(1)
	;// Z1c_2 - in st(2)

	fxch					;// Swap Z1 and Z1b_2.
	fld	st(1)				;// Load Z1.
	fxch					;// Swap Z1 and Z1b_2.
	fld	st(3)				;// Load Z1c_2.
	;// The coprocessor stack looks like this:
	;// Z1c_2 - in st(0)
	;// Z1b_2 - in st(1)
	;// Z1    - in st(2)
	;// Z1    - in st(3)
	;// Z1c_2 - in st(4)

	fsubp	st(3), st			;// Z1C_2 = Z1-Z1c_2
	fsub	st(1), st			;// Z1B_2 = Z1-Z1b_2
	faddp	st(3), st			;// Z1v_2 = Z1b_2+Z1c_2
	;// The coprocessor stack looks like this:
	;// Z1B_2 - in st(0)
	;// Z1C_2 - in st(1)
	;// Z1v_2 - in st(2)

	fld1					;// Load 1.
	fsub	st, st(3)			;// (1 - Z1v_2)
	fstp	qword ptr (0*4*8 + 0*8)[ecx]	;// (*r)[0][0] = (real)(1 - Z1v_2);

	fld1					;// Load 1.
	fsub	st, st(1)			;// (1 - Z1B_2)
	fstp	qword ptr (1*4*8 + 1*8)[ecx]	;// (*r)[1][1] = (real)(1 - Z1B_2);

	fld1					;// Load 1.
	fsub	st, st(2)			;// (1 - Z1C_2)
	fstp	qword ptr (2*4*8 + 2*8)[ecx]	;// (*r)[2][2] = (real)(1 - Z1C_2);
	;// The coprocessor stack looks like this:
	;// Z1B_2 - in st(0)
	;// Z1C_2 - in st(1)
	;// Z1v_2 - in st(2)

	mov	eax, PutFromPtr		;// Load a pointer to the PutFrom data.
	fld	qword ptr (0*8)[eax]	;// t1 = putFrom[0][0].
	fld	qword ptr (1*8)[eax]	;// t2 = putFrom[0][1].
	fld	qword ptr (2*8)[eax]	;// t3 = putFrom[0][2].
	;// The coprocessor stack looks like this:
	;// t3    - in st(0)
	;// t2    - in st(1)
	;// t1    - in st(2)
	;// Z1B_2 - in st(3)
	;// Z1C_2 - in st(4)
	;// Z1v_2 - in st(5)

	;//(*r)[3][0] = (real)( t1*Z1v_2 - t2*(*r)[1][0] - t3*(*r)[2][0] );
	fld	st(2)				;// Load t1.
	fmul	st, st(6)			;// t1*Z1v_2
	fld	qword ptr (1*4*8 + 0*8)[ecx]	;// Load (*r)[1][0].
	fmul	st, st(3)			;// t2*(*r)[1][0]
	fsubp	st(1), st			;// t1*Z1v_2 - t2*(*r)[1][0]
	fld	qword ptr (2*4*8 + 0*8)[ecx]	;// Load (*r)[2][0].
	fmul	st, st(2)			;// t3*(*r)[2][0]
	fsubp	st(1), st			;// t1*Z1v_2 - t2*(*r)[1][0] - t3*(*r)[2][0]
	fstp	qword ptr (3*4*8 + 0*8)[ecx]	;// Write to (*r)[3][0].
	;// The coprocessor stack looks like this:
	;// t3    - in st(0)
	;// t2    - in st(1)
	;// t1    - in st(2)
	;// Z1B_2 - in st(3)
	;// Z1C_2 - in st(4)
	;// Z1v_2 - in st(5)

	;//(*r)[3][1] = (real)( t2*Z1B_2 - t3*(*r)[2][1] - t1*(*r)[0][1] );
	fld	st(1)				;// Load t2.
	fmul	st, st(4)			;// t2*Z1B_2
	fld	qword ptr (2*4*8 + 1*8)[ecx]	;// Load (*r)[2][1].
	fmul	st, st(2)			;// t3*(*r)[2][1]
	fsubp	st(1), st			;// t2*Z1B_2 - t3*(*r)[2][1]
	fld	qword ptr (0*4*8 + 1*8)[ecx]	;// Load (*r)[0][1].
	fmul	st, st(4)			;// t1*(*r)[0][1]
	fsubp	st(1), st			;// t2*Z1B_2 - t3*(*r)[2][1] - t1*(*r)[0][1]
	fstp	qword ptr (3*4*8 + 1*8)[ecx]	;// Write to (*r)[3][1].
	;// The coprocessor stack looks like this:
	;// t3    - in st(0)
	;// t2    - in st(1)
	;// t1    - in st(2)
	;// Z1B_2 - in st(3)
	;// Z1C_2 - in st(4)
	;// Z1v_2 - in st(5)

	;//(*r)[3][2] = (real)( t3*Z1C_2 - t1*(*r)[0][2] - t2*(*r)[1][2] );
	fld	st(0)				;// Load t3.
	fmul	st, st(5)			;// t3*Z1C_2
	fld	qword ptr (0*4*8 + 2*8)[ecx]	;// Load (*r)[0][2].
	fmul	st, st(4)			;// t1*(*r)[0][2]
	fsubp	st(1), st			;// t3*Z1C_2 - t1*(*r)[0][2]
	fld	qword ptr (1*4*8 + 2*8)[ecx]	;// Load (*r)[1][2].
	fmul	st, st(3)			;// t2*(*r)[1][2]
	fsubp	st(1), st			;// t3*Z1C_2 - t1*(*r)[0][2] - t2*(*r)[1][2]
	fstp	qword ptr (3*4*8 + 2*8)[ecx]	;// Write to (*r)[3][2].
	;// The coprocessor stack looks like this:
	;// t3    - in st(0)
	;// t2    - in st(1)
	;// t1    - in st(2)
	;// Z1B_2 - in st(3)
	;// Z1C_2 - in st(4)
	;// Z1v_2 - in st(5)

	fcompp		   			;// Pop two registers.
	;// t1    - in st(0)
	;// Z1B_2 - in st(1)
	;// Z1C_2 - in st(2)
	;// Z1v_2 - in st(3)

	;//(*r)[0][3] = 0.0;
	fldz
	fst	qword ptr (0*4*8 + 3*8)[ecx]
	;//(*r)[1][3] = 0.0;
	fst	qword ptr (1*4*8 + 3*8)[ecx]
	;//(*r)[2][3] = 0.0;
	fstp	qword ptr (2*4*8 + 3*8)[ecx]
	;//(*r)[3][3] = 1.0;
	fld1
	fstp	qword ptr (3*4*8 + 3*8)[ecx]
	;// t1    - in st(0)
	;// Z1B_2 - in st(1)
	;// Z1C_2 - in st(2)
	;// Z1v_2 - in st(3)

pop_4_and_finish:
	;// Free the coprocessor stack.
	fcompp
	fcompp
	;// Free the memory stack.
	popad
}	// __asm

#else	// I387_32
  //Normalization.
  real a=about[0][0];   real b=about[0][1];   real c=about[0][2];
  real v = a*a+b*b+c*c;

  if ( v != 0.0 )
  {
	v=pow(v,0.5);
	a/=v; b/=v; c/=v;
	// Normalization done.

	real Z1  = 1 - cos(through);
	real z2 = sin(through);

	real ABZ1  = a*b*Z1;
	real CZ2 = c*z2;
	(*r)[0][1] = (real)(ABZ1 - CZ2);
	(*r)[1][0] = (real)(ABZ1 + CZ2);

	real ACZ1  = a*c*Z1;
	real BZ2 = b*z2;
	(*r)[0][2] = (real)(ACZ1 + BZ2);
	(*r)[2][0] = (real)(ACZ1 - BZ2);

	real BCZ1  = b*c*Z1;
	real AZ2 = a*z2;
	(*r)[1][2] = (real)(BCZ1 - AZ2);
	(*r)[2][1] = (real)(BCZ1 + AZ2);

	real b_2 = b*b;
	real c_2 = c*c;
  	real Z1b_2 = Z1*b_2;
  	real Z1c_2 = Z1*c_2;
	real Z1B_2 = Z1-Z1b_2;
	real Z1C_2 = Z1-Z1c_2;
	real Z1v_2 = Z1b_2+Z1c_2;

	(*r)[0][0] = (real)(1 - Z1v_2);
	(*r)[1][1] = (real)(1 - Z1B_2);
	(*r)[2][2] = (real)(1 - Z1C_2);

	real t1=putFrom[0][0]; real t2=putFrom[0][1]; real t3=putFrom[0][2];
	(*r)[3][0] = (real)( t1*Z1v_2 - t2*(*r)[1][0] - t3*(*r)[2][0] );
	(*r)[3][1] = (real)( t2*Z1B_2 - t3*(*r)[2][1] - t1*(*r)[0][1] );
	(*r)[3][2] = (real)( t3*Z1C_2 - t1*(*r)[0][2] - t2*(*r)[1][2] );

	(*r)[0][3] = 0.0;
	(*r)[1][3] = 0.0;
	(*r)[2][3] = 0.0;
	(*r)[3][3] = 1.0;
  }
#endif	// I387_32

  if ( v != 0.0 )
  {
	if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
	if ( about.status() == STATUS::TEMPORARY )   delete &about;
	(*this) *= *r;
  }
  else
  {
	mError::set( MERR_ZERO_LENGTH );
	mError::message("Do not know how to rotate about a zero length vector","mTransformer::rotation");
	if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
	if ( about.status() == STATUS::TEMPORARY )   delete &about;
  }

  delete r;
}

void mTransformer::rotate(real through, mLine& about )
{
mPoint p0 = about.point(0);
mPoint p1 = about.point(1);
mPoint vector(p1.x()-p0.x(), p1.y()-p0.y(), p1.z()-p0.z());
	rotate( through, vector, p0 );
}

void mTransformer::rotateX(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","rotationX");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","rotationX");
    delete r;
    return;
  }      

  (*r)[1][1] = (*r)[2][2] = (real) cos(through);
  (*r)[2][1] = (*r)[1][2] = (real) sin(through);
  (*r)[1][2] *= (real) -1.0;
  (*this) *= *r;
  delete r;
}  

void mTransformer::rotateY(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","rotationY");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","rotationY");
    delete r;
    return;
  }      

  (*r)[0][0] = (*r)[2][2] = (real) cos(through);
  (*r)[2][0] = (*r)[0][2] = (real) sin(through);
  (*r)[2][0] *= (real) -1.0;
  (*this) *= *r;
  delete r;
}  

void mTransformer::rotateZ(real through)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","rotationZ");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","rotationZ");
    delete r;
    return;
  }      

  (*r)[0][0] = (*r)[1][1] = (real) cos(through);
  (*r)[1][0] = (*r)[0][1] = (real) sin(through);
  (*r)[0][1] *= (real) -1.0;
  (*this) *= *r;
  delete r;
}  
 
void mTransformer::translate(matrix& to, matrix& putFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 2 );
    mError::message("Wrong second argument","mTransformer::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( to.status() == STATUS::TEMPORARY )      delete &to;
    return;
  }
  if ( to[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 1 );
    mError::message("Wrong first argument","mTransformer::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    if ( to.status() == STATUS::TEMPORARY )      delete &to;
    return;
  }                          

  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","translation");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","translation");
    delete r;
    return;
  }      

  (*r)[3][0] = (real) ( to[0][0] - putFrom[0][0] );
  (*r)[3][1] = (real) ( to[0][1] - putFrom[0][1] );
  (*r)[3][2] = (real) ( to[0][2] - putFrom[0][2] );

  if ( to.status()== STATUS::TEMPORARY ) delete &to;
  if ( putFrom.status()== STATUS::TEMPORARY ) delete &putFrom;
  (*this) *= *r;
  delete r;
}

void mTransformer::translate(real x,real y,real z, matrix& putFrom)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  if ( putFrom[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 4 );
    mError::message("Wrong fourth argument","mTransformer::translation");
    if ( putFrom.status() == STATUS::TEMPORARY ) delete &putFrom;
    return;
  }

  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","translation");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","translation");
    delete r;
    return;
  }      

  (*r)[3][0] = (real) ( x-putFrom[0][0] );
  (*r)[3][1] = (real) ( y-putFrom[0][1] );
  (*r)[3][2] = (real) ( z-putFrom[0][2] );

  if ( putFrom.status()== STATUS::TEMPORARY ) delete &putFrom;
  (*this) *= *r;
  delete r;
}  
  
void mTransformer::scale(real Kx,real Ky,real Kz, matrix& about)
//William M. Newman, Robert F. Sproull; 
//Principles of interactive computer graphics, McGRAW-HILL
//Chapter22:
{
  mError::set();
  if ( about[0] == NULL )
  {
    mError::set( MERR_WRONG_ARGUMENT, 4 );
    mError::message("Wrong fourth argument","mTransformer::scaling");
    if ( about.status() == STATUS::TEMPORARY )   delete &about;
    return;
  }                          

  mTransformer *r = new mTransformer;
  if ( r == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 1 );
    mError::message("Not enough memory","scaling");
    return;
  }      
  if ( (*r)[0] == NULL )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY, 2 );
    mError::message("Not enough memory","scaling");
    delete r;
    return;
  }      

  (*r)[0][0] = (real) Kx;
  (*r)[1][1] = (real) Ky;
  (*r)[2][2] = (real) Kz;
  (*r)[3][0] = (real) ( about[0][0]*(1.0-Kx) );
  (*r)[3][1] = (real) ( about[0][1]*(1.0-Ky) );
  (*r)[3][2] = (real) ( about[0][2]*(1.0-Kz) );
  (*r)[3][3] = (real) 1.0;
   
  if ( about.status()== STATUS::TEMPORARY ) delete &about;
  (*this) *= *r;
  delete r;
}  

// This function is purposed to hide the multiplication
// operator in case I decide to speed up calculations
// using different algorithm (transposed transformer)
void mTransformer::concatenate( mTransformer& t )
{
	*this *= t;
}

mTransformable::mTransformable(int rows) : matrix(rows, 4)
{
real* element = (*this)[0] + 3;
real* top = element + rows*4;
int step = 4;
	while( element < top )
	{
		*element = 1;
		element += step;
	}
}

mTransformable::mTransformable(real init_value, int rows) : matrix(init_value, rows, 4)
{
real* element = (*this)[0] + 3;
real* top = element + rows*4;
int step = 4;
	while( element < top )
	{
		*element = 1;
		element += step;
	}
}

mTransformable::mTransformable(mTransformable& t) : matrix( t ) {}

mTransformable::mTransformable( void ) : matrix() {}

void mTransformable::transform( mTransformer& t )
{
	(*this)*=t;
}

