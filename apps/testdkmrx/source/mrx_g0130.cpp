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
	group = 130;
	test = 1;
	description = "Copy constructor assisted by assignment.";
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;
	real realInitialValue = 3.0;

	matrix mrx1(iRows, iColumns, realInitialValue);
	real realSum1 = getSum(mrx1);

	matrix mrx2 = mrx1;
	real realSum2 = getSum(mrx2);

	bPassed =
		(mrx1.rows() == iRows) &&
		(mrx1.columns() == iColumns) &&
		(mrx2.rows() == iRows) &&
		(mrx2.columns() == iColumns) &&
		(realSum1 == realSum2);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 130;
	test = 2;
	description = "Straight copy assignment.";
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;
	real realInitialValue = 3.0;

	matrix mrx1(iRows, iColumns, realInitialValue);
	real realSum1 = getSum(mrx1);

	matrix mrx2;
	mrx2 = mrx1;
	real realSum2 = getSum(mrx2);

	bPassed =
		(mrx1.rows() == iRows) &&
		(mrx1.columns() == iColumns) &&
		(mrx2.rows() == iRows) &&
		(mrx2.columns() == iColumns) &&
		(realSum1 == realSum2);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 130;
	test = 3;
	description = "Move constructor assisted by assignment.";
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;
	real realInitialValue = 3.0;

	matrix mrx1(iRows, iColumns, realInitialValue);
	real realSum1 = getSum(mrx1);

	matrix mrx2 = std::move(mrx1);
	real realSum2 = getSum(mrx2);

	bPassed =
		(mrx1.rows() == 0) &&
		(mrx1.columns() == 0) &&
		(mrx2.rows() == iRows) &&
		(mrx2.columns() == iColumns) &&
		(realSum1 == realSum2);

	return bPassed;
}
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	group = 130;
	test = 4;
	description = "Straight move assignment.";
}
bool execute()
{
	bool bPassed{ false };

	int iRows = 3;
	int iColumns = 4;
	real realInitialValue = 3.0;

	matrix mrx1(iRows, iColumns, realInitialValue);
	real realSum1 = getSum(mrx1);

	matrix mrx2;
	mrx2 = std::move(mrx1);
	real realSum2 = getSum(mrx2);

	bPassed =
		(mrx1.rows() == 0) &&
		(mrx1.columns() == 0) &&
		(mrx2.rows() == iRows) &&
		(mrx2.columns() == iColumns) &&
		(realSum1 == realSum2);

	return bPassed;
}
END_UNIT_TEST
