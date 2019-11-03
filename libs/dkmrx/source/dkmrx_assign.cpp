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

matrix& matrix::operator=(const matrix& mx)
{
	mError::set();

	if( mx.pValues_ == nullptr)
	{
	    empty();
	    return *this;
	}

	if ( pValues_ != nullptr) delete [] pValues_;
	iRows_=mx.iRows_;
	iColumns_=mx.iColumns_;
	size_t MatSize = mx.iRows_ * mx.iColumns_;
	pValues_= new real[MatSize];
	if ( pValues_ == nullptr)
	{
	    mError::set( MERR_INSUFFICIENT_MEMORY );
	    mError::message("Not enough memory","matrix::operator=(const matrix& mx)");
	    iRows_=iColumns_=0;
	    return *this;
	}
	real *index1,*index2,*top;
	index1= pValues_;
	index2= mx.pValues_;
	top   = index1 + MatSize;
	while ( index1<top ) *index1++ = *index2++;
	return *this;
}

matrix& matrix::operator=(matrix&& mx) noexcept
{
	mError::set();

	if (mx.pValues_ == nullptr)
	{
		empty();
		return *this;
	}

	if (pValues_ != nullptr) delete[] pValues_;
	iRows_ = mx.iRows_;
	iColumns_ = mx.iColumns_;
	pValues_ = mx.pValues_;

	mx.pValues_ = nullptr;
	mx.iRows_ = 0;
	mx.iColumns_ = 0;
	return *this;
}
