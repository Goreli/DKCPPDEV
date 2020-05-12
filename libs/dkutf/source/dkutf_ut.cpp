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
#include "dkutf_ut.hpp"

using namespace dk;

// UnitTest objects are meant to be defined in the global scope.
// Use the singleton pattern to initialise the list in a controlled
// manner during the first invocation of the constructor.
static UTList& singleton() {
	static UTList list_;
	return list_;
}

UnitTest::UnitTest()
	: description_{ "" }, test_{ 0 }, group_{ 0 }, utkCompositeKey_{0, 0}
{
	auto& list = singleton();
	list.push_back(this);
}
UnitTest::~UnitTest()
{}
static bool comparisonFunction(UnitTest* p1, UnitTest* p2)
{
	return p1->getKey() < p2->getKey();
}
void UnitTest::sort() noexcept
{
	auto& list = singleton();
	std::sort(list.begin(), list.end(), comparisonFunction);
}
const UTList& UnitTest::list() noexcept
{
	return singleton();
}
void UnitTest::initCompositeKey() noexcept
{
	utkCompositeKey_ = UTKey(group_, test_);
}
const UTKey& UnitTest::getKey() const noexcept
{
	return utkCompositeKey_;
}
const std::string& UnitTest::getDescription() const noexcept
{
	return description_;
}

void UnitTest::_describe(const std::string& d, unsigned t, unsigned g) noexcept
{
	description_ = d;
	test_ = t;
	group_ = g;
}

