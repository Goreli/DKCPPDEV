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

	//enum class STATUS { PERMANENT, TEMPORARY };
	enum class STORE_OPERATION { SAVE, LOAD };

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

		matrix operator +      (const matrix&) const;
		matrix operator +      (double) const;
		matrix operator -      (const matrix&) const;
		matrix operator -      (double) const;
		matrix operator *      (const matrix&) const;
		matrix operator *      (double) const;
		matrix& operator *=     (const matrix&);
		matrix operator /      (matrix&) const;	// Need to make the parameter const. There is a dependency on storeValues - sort out why.
		matrix& operator /=     (matrix&);	// Need to make the parameter const. There is a dependency on storeValues - sort out why.
		matrix operator ~      (void);	// Need to make the overloaded operator const. Depends on operator /=.
		void     normalise(void);
		void     minmax(real* min, real* max);
		real     average(void);
		real     mean_square(void);
		real     det(void);
		real     trace(void);
		real* operator [] (int i) const { return Values + (((long long)Columns) * ((long long)i)); }
		int      transpose(void);
		matrix& extract(int FirstRow, int FirstCol,
			int RowsTotal, int ColsTotal);
		void     paste_and_add(int FirstToRow, int FirstToCol,
			matrix& From,
			int FirstFromRow, int FirstFromCol,
			int RowsTotal, int ColsTotal);
		friend  std::ostream& operator <<    (std::ostream&, matrix&);
		friend  std::istream& operator >>    (std::istream&, matrix&);
		void     empty(void); // Frees occupied memory
		int      is_empty(void) { return !Rows; }
		void     name(const char*);
		char* name(void) { return Name; }
		int      columns(void) const { return Columns; }
		int      rows(void) const { return Rows; }
		//STATUS   status(void) { return Status; }
		//void     status(STATUS s) { Status = s; }
		static  matrix identity(int);
		// Homogeneous group
		int             toHG(real = 1.0);
		int             fromHG(void);

		// The compression group of functions.
		void	shortenSignificand(real precision);	// Reduces the number of non-zero bits in the IEEE significand.
		void	shortenSignificand(real precision, real min, real max);

#if defined(MATRIX_DEBUG)
		static	int	created(void) { return Created; }
		static	int	destroyed(void) { return Destroyed; }
#endif

	private:

		real* Values;
		//STATUS  Status;
		int     Rows, Columns;
		struct { real* MemoryArea; char* FileName; } Storage;
		char* Name;

#if defined(MATRIX_DEBUG)
		static	int	Created;
		static	int	Destroyed;
#endif

	protected:

		int  storeValues(STORE_OPERATION);
		void values(real* v) { if (Values) delete[] Values; Values = v; }
	};

} // namespace dkmrx
#endif // dkmrx_matrix_hpp
