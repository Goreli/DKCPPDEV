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
#include <utility>
#include "dkutf_ut_macros.hpp"
#include "testdkmrx_util.hpp"
#include "dkmrx_matrix.hpp"

using namespace dkmrx;
using namespace dk;

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "One default constructor to test the ConstDestTracker instrumentation.";
	test = 1;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Two default constructors to test the ConstDestTracker instrumentation.";
	test = 2;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx1;
	matrix mrx2;

	bPassed = true;

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Three default constructors to test the ConstDestTracker instrumentation.";
	test = 3;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx1;
	matrix mrx2;
	matrix mrx3;

	bPassed = true;

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Parameterised constructor: test the rows and columns parameters.";
	test = 4;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;

	matrix mrx(iRows, iColumns);

	bPassed = (mrx.rows() == iRows) && (mrx.columns() == iColumns);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Parameterised and copy constructors: test the rows and columns parameters.";
	test = 5;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;

	matrix mrx1(iRows, iColumns);
	matrix mrx2(mrx1);

	bPassed =
		(mrx1.rows() == iRows) &&
		(mrx1.columns() == iColumns) &&
		(mrx2.rows() == iRows) &&
		(mrx2.columns() == iColumns);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Parameterised, copy and move constructors: test the rows and columns parameters.";
	test = 6;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;

	matrix mrx1(iRows, iColumns);
	matrix mrx2(mrx1);
	matrix mrx3(std::move(mrx2));

	bPassed =
		(mrx1.rows() == iRows) &&
		(mrx1.columns() == iColumns) &&
		(mrx2.rows() == 0) &&
		(mrx2.columns() == 0) &&
		(mrx3.rows() == iRows) &&
		(mrx3.columns() == iColumns);	

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Initialising constructor: test the rows, columns and initial value parameters.";
	test = 7;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;
	real realInitialValue = 3.0;

	matrix mrx(iRows, iColumns, realInitialValue);
	real realSum = getSum(mrx);

	bPassed =
		(mrx.rows() == iRows) &&
		(mrx.columns() == iColumns) &&
		(realSum == iRows * iColumns * realInitialValue);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Uniform initialisation: matrix(const std::initializer_list<const std::initializer_list<real>>&)";
	test = 8;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx{
		{1.3, 2.5, 3.2},
		{4.1, 5.2, 6.7},
		{7,   8,   9},
		{6,   5,   4},
		{3,   2,   1}
	};
	real expectedValue = real(68.0);
	real realSum = getSum(mrx);

	bPassed =
		(mrx.rows() == 5) &&
		(mrx.columns() == 3) &&
		(realSum == expectedValue);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Uniform initialisation: matrix(size_t rows, const std::initializer_list<real>&)";
	test = 9;
	group = 120;
}
bool execute()
{
	bool bPassed{ false };

	matrix mrx(5, {
		1.3, 2.5, 3.2,
		4.1, 5.2, 6.7,
		7,   8,   9,
		6,   5,   4,
		3,   2,   1
	});
	real expectedValue = real(68.0);
	real realSum = getSum(mrx);

	bPassed =
		(mrx.rows() == 5) &&
		(mrx.columns() == 3) &&
		(realSum == expectedValue);

	return bPassed;
}
END_UNIT_TEST
