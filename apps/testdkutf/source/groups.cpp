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

#include "dkutf_utg_macros.hpp"

/*
Use the following macro to create a group of unit tests:

UNIT_TEST_GROUP(const BasePositiveInteger& groupId, const std::string& description)

Parameters:
	groupId - a unique id of a group of unit tests. 
	  If the id is not unique the program won't spit the dummy.
		However, it will ignore unit tests pertaining to the duplicate
		group and, if in the validation mode, report the group as a duplicate.
	  Do not use 0 (zero) as a group id as this number is reserved
		for the system default group. Custom groups defined using
		the reserved id will be ignored.

	description - the description of the group of unit tests. 
*/

UNIT_TEST_GROUP(100, "Macro syntax scenarios")
UNIT_TEST_GROUP(110, "ConstDestTracker default constructor")
UNIT_TEST_GROUP(120, "ConstDestTracker copy constructor")
UNIT_TEST_GROUP(130, "ConstDestTracker move constructor")
UNIT_TEST_GROUP(140, "ConstDestTracker overloaded assignment operators")
