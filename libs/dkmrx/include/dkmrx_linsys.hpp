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
