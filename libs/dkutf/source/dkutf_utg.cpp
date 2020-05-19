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

#include <algorithm>
#include "dkutf_utg.hpp"

using namespace dk;

// UnitTestGroup objects are meant to be defined in the global scope.
// Use the singleton pattern to initialise the list in a controlled
// manner during the first invocation of the constructor.
static UTGList& singleton() {
	static UTGList list_;
	return list_;
}

// This is the first group object defined in the global scope right
// here, in the library module. The purpose of this group is to provide
// a system default group that users could assign unit tests to with
// minimum effort without the need of explicitly defined groups.
UnitTestGroup UnitTestGroup::objDefaultGroup_;

UnitTestGroup::UnitTestGroup(const BasePositiveInteger& objPosInt, const std::string& sDescription)
	: uiGroup_{ unsigned int (objPosInt) }, sDescription_{ sDescription }
{
	auto& list = singleton();
	list.push_back(this);
}
UnitTestGroup::UnitTestGroup()
	: uiGroup_{0}, sDescription_{ "Default group" }
{
	auto& list = singleton();
	list.push_back(this);
}
UnitTestGroup::~UnitTestGroup()
{}
static bool comparisonFunction(const UnitTestGroup* p1, const UnitTestGroup* p2)
{
	return p1->group() < p2->group();
}
void UnitTestGroup::sort() noexcept
{
	auto& list = singleton();
	std::sort(list.begin(), list.end(), comparisonFunction);
}
const UTGList& UnitTestGroup::list() noexcept
{
	return singleton();
}
unsigned int UnitTestGroup::group() const noexcept
{
	return uiGroup_;
}
const std::string& UnitTestGroup::description() const noexcept
{
	return sDescription_;
}
