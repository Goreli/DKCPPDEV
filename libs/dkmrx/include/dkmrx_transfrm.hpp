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

#ifndef libs_dkmrx_transfrm_hpp
#define libs_dkmrx_transfrm_hpp

#include "dkmrx_matrix.hpp"

namespace dkmrx {

	class mLine;

	class mTransformer : public matrix
	{
	public:
		mTransformer(void);
		mTransformer(const mTransformer&);

		void rotate(real through, const matrix& about, const matrix& putFrom);
		void rotate(real through, const mLine& about);
		void rotateX(real through);
		void rotateY(real through);
		void rotateZ(real through);
		void translate(const matrix& to, const matrix& putFrom);
		void translate(real x, real y, real z, const matrix& putFrom);
		void scale(real Kx, real Ky, real Kz, const matrix& about);
		void reset(void);
	};

	class mTransformable : public matrix
	{
	public:
		mTransformable(int dim);
		mTransformable(int dim, real init_value);
		mTransformable(const mTransformable&);
		mTransformable(void);

		virtual	void transform(const mTransformer&);
		//matrix& operator = (const matrix& m) { return matrix::operator=(m); }
	};

} // namespace dkmrx
#endif // libs_dkmrx_transfrm_hpp
