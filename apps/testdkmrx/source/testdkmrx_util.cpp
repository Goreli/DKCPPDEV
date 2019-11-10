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

#include "testdkmrx_util.hpp"
#include "dkmrx_matrix.hpp"

using namespace dkmrx;

real calcProdOfMainDiag(const matrix& mrx)
{
	real prod = real(1.0);
	for (size_t iRow = 0; iRow < mrx.rows(); iRow++)
		prod *= mrx[iRow][iRow];
	return prod;
}

bool isUpperTriangular(const matrix& mrx)
{
	for (size_t iRow = 1; iRow < mrx.rows(); iRow++)
		for (size_t iCol = 0; iCol < iRow; iCol++)
			if (mrx[iRow][iCol] != 0.0)
				return false;
	return true;
}

real getSum(const matrix& mrx)
{
	real realSum = 0.0;
	for (int iRow = 0; iRow < mrx.rows(); iRow++)
		for (int iColumn = 0; iColumn < mrx.columns(); iColumn++)
			realSum += mrx[iRow][iColumn];
	return realSum;
}

