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

#include <iostream>
#include "dkmrx_matrix.hpp"
#include "dk_ut_macros.hpp"

using namespace dkmrx;
using namespace dk;

// This function calculates the product of matrix elements
// located on the main diagonal.
static real calcProdOfMainDiag(const matrix& mrx)
{
	real prod = real(1.0);
	for (size_t iRow = 0; iRow < mrx.rows(); iRow++)
		prod *= mrx[iRow][iRow];
	return prod;
}

static bool isUpperTriangular(matrix mrx)
{
	for (size_t iRow = 1; iRow < mrx.rows(); iRow++)
		for (size_t iCol = 0; iCol < iRow; iCol++)
			if (mrx[iRow][iCol] != 0.0)
				return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 190;
	test = 1;
	description = "Call makeUpperTriangular() for a 1x1 matrix and calculate the product of diagonal elements.";
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx(1, 1);
	mrx[0][0] = 7;

	mrx.makeUpperTriangular();


	// Check the determinant - it should equal 7.0.
	bPassed = (real(7.0) == mrx[0][0]);

	return bPassed;
}
END_UNIT_TEST


///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 190;
	test = 2;
	description = "Call makeUpperTriangular() for a 2x2 matrix and calculate the product of diagonal elements.";
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx(2, 2);
	mrx[0][0] = 4; mrx[0][1] = 6;
	mrx[1][0] = 3; mrx[1][1] = 8;

	mrx.makeUpperTriangular();

	// Check the determinant - it should equal 14.0.
	// Refer https://www.mathsisfun.com/algebra/matrix-determinant.html
	bPassed = (real(14.0) == calcProdOfMainDiag(mrx)) && isUpperTriangular(mrx);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 190;
	test = 3;
	description = "Call makeUpperTriangular() for a 3x3 matrix and calculate the product of diagonal elements.";
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx{
		{6,  1, 1},
		{4, -2, 5},
		{2,  8, 7}
	};

	mrx.makeUpperTriangular();

	// Check the determinant - it should equal -306.0.
	// Refer https://www.mathsisfun.com/algebra/matrix-determinant.html
	bPassed = (real(-306.0) == calcProdOfMainDiag(mrx)) && isUpperTriangular(mrx);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 190;
	test = 4;
	description = "Call makeUpperTriangular() for a 4x4 matrix and calculate the product of diagonal elements.";
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx{ 4, {
		4, 7, 2, 3,
		1, 3, 1, 2,
		2, 5, 3, 4,
		1, 4, 2, 3
	} };

	mrx.makeUpperTriangular();

	// Check the determinant - it should equal -3.
	// Refer example #40 on page https://www.math10.com/en/algebra/matrices/determinant.html
	bPassed = (real(-3.0) == calcProdOfMainDiag(mrx)) && isUpperTriangular(mrx);
	
	return bPassed;
}
END_UNIT_TEST
