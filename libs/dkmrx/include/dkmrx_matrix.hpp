/*
  General purpose matrix library, Version 1.0
  Copyright (c) 1994 David Krikheli
 
  Permission to use, copy, modify, distribute, and sell this software and 
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notice and this permission notice appear in
  all copies of the software and related documentation, and (ii) the name of
  David Krikheli may not be used in any advertising or publicity relating to
  the software without the specific, prior written permission of David Krikheli.
  
  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 
  IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
  INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
  POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR 
  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef Matrix_h
#define Matrix_h

#include <iostream>

using std::ostream;
using std::istream;


enum class STATUS{ PERMANENT, TEMPORARY };
enum class STORE_OPERATION{ SAVE, LOAD };
    
class matrix
{     
public:

	matrix(void);
	matrix(int rows,int columns);
	matrix(real init_value, int rows, int columns);   
	matrix(matrix&);
	~matrix();

	matrix& operator +      (matrix&);
	matrix& operator +      (double);
	matrix& operator -      (matrix&);
	matrix& operator -      (double);
	matrix& operator *      (matrix&);
	matrix& operator *      (double);
	matrix& operator *=     (matrix&);
	matrix& operator /      (matrix&);
	matrix& operator /=     (matrix&);  
	matrix& operator ~      (void);
virtual	matrix& operator =      (matrix&);
	void     normalise      (void);
	void     minmax         (real* min, real* max);
	real     average        (void);
	real     mean_square    (void);
	real     det            (void);
	real     trace          (void);
	real*    operator []    (int i) { return Values+(((int64_t)Columns)*((int64_t)i));}
	int      transpose      (void);
	matrix&  extract        (int FirstRow, int FirstCol,
				 int RowsTotal,  int ColsTotal);
	void     paste_and_add  (int FirstToRow, int FirstToCol,
				 matrix& From,
				 int FirstFromRow, int FirstFromCol,
				 int RowsTotal,  int ColsTotal);
friend  ostream& operator <<    (ostream&,matrix&);
friend  istream& operator >>    (istream&,matrix&);
	void     empty          (void); // Frees occupied memory
	int      is_empty       (void) { return !Rows; }
	void     name           (const char*);
	char*    name           (void)  { return Name; }   
	int      columns        (void)  { return Columns; }
	int      rows           (void)  { return Rows; }
	STATUS   status         (void)  { return Status;}
	void     status         (STATUS s)      { Status=s;}
static  matrix&  identity       (int);
// Homogeneous group
	int             toHG            (real =1.0);
	int             fromHG          (void);

	// The compression group of functions.
	void	shortenSignificand(real precision);	// Reduces the number of non-zero bits in the IEEE significand.
	void	shortenSignificand(real precision, real min, real max);

#if defined(MATRIX_DEBUG)
static	int	created(void) { return Created; }
static	int	destroied(void) { return Destroied; }
#endif

private:     

	real  *Values;
	STATUS  Status;
	int     Rows, Columns;
	struct{ real* MemoryArea; char* FileName; } Storage;
	char   *Name;

#if defined(MATRIX_DEBUG)
	static	int	Created;
	static	int	Destroied;
#endif

protected:

	int  storeValues(STORE_OPERATION);
	void values( real* v ) { if( Values ) delete [] Values; Values = v; }
};

#endif //Matrix_h
