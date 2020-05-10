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

UnitTestGroup::UnitTestGroup(unsigned uKey, const std::string& sDescription)
	: uKey_{ uKey }, sDescription_{ sDescription }
{
	auto& list = singleton();
	list.push_back(this);
}
UnitTestGroup::~UnitTestGroup()
{}
static bool comparisonFunction(UnitTestGroup* p1, UnitTestGroup* p2)
{
	return p1->getKey() < p2->getKey();
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
unsigned UnitTestGroup::getKey() const noexcept
{
	return uKey_;
}
const std::string& UnitTestGroup::getDescription() const noexcept
{
	return sDescription_;
}
