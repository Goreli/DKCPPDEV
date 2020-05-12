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

#include "dkutf_ut_macros.hpp"

///////////////////////////////////////////////////////////////////////////////
BEGIN_UNIT_TEST
void describe()
{
	description = "Explicit move constructor invocation:"
				" ConstDestTracker obj1(std::move(obj2));";
	test = 1;
	group = 130;
}
bool execute()
{
	bool bPassed{ false };

	//dk::ConstDestTracker obj2;
	//dk::ConstDestTracker obj1(std::move(obj2));

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
	description = "Assignment operator assisted move constructor invocation:"
					" ConstDestTracker obj1 = std::move(obj2);";
	test = 2;
	group = 130;
}
bool execute()
{
	bool bPassed{ false };

	//dk::ConstDestTracker obj2;
	//dk::ConstDestTracker obj1 = std::move(obj2);

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
}
END_UNIT_TEST
