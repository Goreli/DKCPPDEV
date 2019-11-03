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

#ifndef dkmrx_matrix_hpp
#define dkmrx_matrix_hpp

#include <iosfwd>

namespace dkmrx {

	class matrix
	{
	public:

		matrix(void);
		matrix(int rows, int columns);
		matrix(real init_value, int rows, int columns);
		matrix(const matrix&);
		matrix(matrix&&) noexcept;

		virtual ~matrix();

		matrix& operator =      (const matrix&);
		matrix& operator =      (matrix&&) noexcept;

		real* operator [] (int i) const { return pValues_ + (((long long)iColumns_) * ((long long)i)); }
		friend  std::ostream& operator <<    (std::ostream&, matrix&);
		friend  std::istream& operator >>    (std::istream&, matrix&);
		
		matrix operator +      (const matrix&) const;
		matrix operator +      (real) const;
		matrix& operator +=    (const matrix&);
		matrix& operator +=    (real);

		matrix operator -      (const matrix&) const;
		matrix operator -      (real) const;
		matrix& operator -=    (const matrix&);
		matrix& operator -=    (real);

		matrix operator *      (const matrix&) const;
		matrix operator *      (real) const;
		matrix& operator *=    (const matrix&);
		matrix& operator *=    (real);

		// A matrix format of a system of simultaneious linear equations
		// normally looks like: A*x=B. From here we will derive the following
		// matrix notation: x=B/A. The overloaded "/" operator will therefore
		// perform Gauss elimination to solve the system.
		// Matrix B data is expected to be written in the *this object.
		// Matrix A data is expected to be written in the A parameter object.
		// Upon completion:
		//  - The returned matrix object contains x;
		//  - Inplace Gauss elimination changes matrix A - cannot make it const ref;
		//  - The *this object containing matrix B remains unchanged. Hence const;
		matrix operator /      (matrix& A) const;

		matrix operator /      (real) const;

		// Assume A*x=B. Then another form of solution could be recorded as B/=A.
		// B/=A -this operator will execute Gauss elimination and write the result
		// to the *this object. In other words Gauss elimination will replace
		// data pertaining to matrix B with data pertaining to the output x matrix.
		// Upon completion:
		//  - The object on the left (and returned ref) no longer contains matrix B
		//    data and now contains data pertaining to the output x matrix instead.
		//  - Inplace Gauss elimination changes matrix A - cannot make it const ref.
		matrix& operator /= (matrix& A);

		matrix& operator /=    (real);

		// Returns an inverse of the *this matrix. Does not preserve the content
		// because inplace Gauss elimination distorts it. Use a separate dedicated
		// matrix if in need to preserve the content.
		matrix operator ~ (void);

		static  matrix identity(int);

		// Homogeneous group
		int             toHG(real = 1.0);
		int             fromHG(void);

		void	empty(void); // Frees occupied memory
		void	name(const char*);
		char*	name(void) { return pName_; }
		bool	is_empty(void) { return iRows_==0; }
		int		columns(void) const { return iColumns_; }
		int		rows(void) const { return iRows_; }

#if defined(MATRIX_TEST)
		static	int	getConstructCount(void) { return iConstructCount_; }
		static	int	getDestructCount(void) { return iDestructCount_; }
		static	int	getControlSum(void) { return iControlSum_; }
		static void resetCountsAndSum() { iConstructCount_ = iDestructCount_ = iControlSum_ = 0;  }
#endif

	private:

		real* pValues_;
		int   iRows_, iColumns_;
		char* pName_;

#if defined(MATRIX_TEST)
		static int iConstructCount_;
		static int iDestructCount_;
		static int iControlSum_;
		int iSeqId_;
		// The following is expected at the end of test execution:
		// 1. iConstructCount_ should equal iDestructCount_.
		//    These counts indicate the total number of matrix objects created;
		// 2. iControlSum_ should equal 0.
		//
		// Use the public static get... functions above to examine the values.
		// Make sure to call resetCountsAndSum() before each test.
#endif
	};

} // namespace dkmrx
#endif // dkmrx_matrix_hpp
