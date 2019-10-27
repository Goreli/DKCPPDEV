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

History:

  30/Mar/02 - DK commented the gauss_elimination declaration out as this function
  has been implemented in a template form.
*/



#ifndef linsys_h
#define linsys_h

#include "dkmrx_matrix.hpp"
//#include "bool.hpp"

//int gauss_elimination(int,int,real*,real*);

/************************************************************\
*                         "linsys"                           *
*   Basic class for solving a simultenious linear equations  *
*                                                            *
\************************************************************/

class linsys
{
public:
	linsys(void);
virtual void	 exclude_rows_and_columns(unsigned* List, unsigned Entries);
virtual void	 solve_in_single_step(void)    = 0;
virtual unsigned solve_in_multiple_steps(void) = 0;// returns progress()
virtual void	 restore_rows_and_columns(void);
	int	 how_many_solutions(void) { return NumberOfSolutions_; }

protected:
	int	 progress(void); //returns 1%-100%
	bool is_excluded(unsigned);
	unsigned ToBeDone_;
	matrix	 *ExcludedRows_;
	matrix	 *ExcludedColumns_;

private:
	unsigned Done_;
	unsigned *ExcludeList_;
	unsigned ExcludeListEntries_;
	int	 NumberOfSolutions_;
};


class linsysGauss : public linsys
{
public:
	linsysGauss(matrix* Left, matrix* Right, bool MultyPass = false);
	void	 exclude_rows_and_columns(unsigned* List, unsigned Entries);
	void	 solve_in_single_step(void);
	unsigned solve_in_multiple_steps(void);
	void	 restore_rows_and_columns(void);
};

class linsysKholesky : public linsys
{
public:
	linsysKholesky(matrix* Left, matrix* Right, bool MultyPass = false);
	void	 exclude_rows_and_columns(unsigned* List, unsigned Entries);
	void	 solve_in_single_step(void);
	unsigned solve_in_multiple_steps(void);
	void	 restore_rows_and_columns(void);
};
   
#endif /* linsys_h */
