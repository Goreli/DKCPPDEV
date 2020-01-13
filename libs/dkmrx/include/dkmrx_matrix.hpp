/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#ifndef libs_dkmrx_matrix_hpp
#define libs_dkmrx_matrix_hpp

#include <iosfwd>

#if defined(DKUTF_CONST_DEST_TRACKER)
#include "dk_cdt.hpp"
#endif

namespace dk {
	class MultiThreadedDriver;
}


namespace dkmrx {

	class matrix
	{
	public:

		matrix(void) noexcept;
		matrix(size_t rows, size_t columns);
		matrix(size_t rows, size_t columns, real init_value);
		matrix(const std::initializer_list<const std::initializer_list<real>>&);
		matrix(size_t rows, const std::initializer_list<real>&);

		matrix(const matrix&);
		matrix(matrix&&) noexcept;

		virtual ~matrix();

		matrix& operator =      (const matrix&);
		matrix& operator =      (matrix&&) noexcept;

		real* operator [] (size_t i) const { return pValues_ + (((long long)iColumns_) * ((long long)i)); }
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
		void multiply_MT(const matrix&, const matrix&, dk::MultiThreadedDriver*);

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

		void makeUpperTriangular();

		// Returns an inverse of the *this matrix. Does not preserve the content
		// because inplace Gauss elimination distorts it. Use a separate dedicated
		// matrix if in need to preserve the content.
		matrix operator ~ ();

		static  matrix identity(size_t);

		void transposedOf(const matrix&);
		void transpose();

		// Homogeneous group
		void	toHG(real = 1.0);
		void	fromHG(void);

		void	empty(void) noexcept; // Frees occupied memory
		bool	is_empty(void) const { return pValues_; }
		size_t	columns(void) const { return iColumns_; }
		size_t	rows(void) const { return iRows_; }

	private:
		real* pValues_;
		size_t iRows_;
		size_t iColumns_;

#if defined(DKUTF_CONST_DEST_TRACKER)
		dk::ConstDestTracker tracker_;
#endif

	protected:
		// Validate function args and throw exceptions if necessary.
		static void _validate(bool, bool, bool, const char*);
		static void _validate(bool, const char*);
	};	// class matrix
}	// namespace dkmrx
#endif	// libs_dkmrx_matrix_hpp
