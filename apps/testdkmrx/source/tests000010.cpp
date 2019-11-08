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


#include "dk_cdt.hpp"
#include "dk_ut_macros.hpp"

using namespace dk;

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 1;
	description = "Default constructor.";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj1;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 2;
	description = "Two default constructors.";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj1;
	ConstDestTracker obj2;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 3;
	description = "Three default constructors.";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj1;
	ConstDestTracker obj2;
	ConstDestTracker obj3;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 4;
	description = "Explicit copy constructor invocation:"
					" ConstDestTracker obj1(obj2);";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1(obj2);

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 5;
	description = "Assignment operator assisted copy constructor invocation:"
					" ConstDestTracker obj1 = obj2;";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1 = obj2;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 6;
	description = "Explicit move constructor invocation:"
					" ConstDestTracker obj1(std::move(obj2));";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1(std::move(obj2));

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 7;
	description = "Assignment operator assisted move constructor invocation:"
					" ConstDestTracker obj1 = std::move(obj2);";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1 = std::move(obj2);

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 8;
	description = "Copy assignment operator: obj1 = obj2;";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1;
	obj1 = obj2;

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST

///////////////////////////////////////////////////////////////////////////////
DESCRIBE_UNIT_TEST
	group = 10;
	test = 9;
	description = "Move assignment operator: obj1 = std::move(obj2);";
DEFINE_UNIT_TEST
	bool bPassed{ false };

	ConstDestTracker obj2;
	ConstDestTracker obj1;
	obj1 = std::move(obj2);

	// For the purpose of this group of tests integrity of the ConstDestTracker
	// object is automatically tested outside this routine. So there is no need
	// to validate anything here.
	bPassed = true;

	return bPassed;
END_UNIT_TEST
