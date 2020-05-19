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
	: description_{ "" }, utkCompositeKey_{0, 0}
{
	auto& list = singleton();
	list.push_back(this);
}
UnitTest::~UnitTest()
{}

// Comparison for std::sort.
static bool compareByGroupNo(UnitTest* p1, UnitTest* p2)
{
	return p1->key().group() < p2->key().group();
}
// heterogeneous comparison for std::equal_range.
struct ComparisonPredicate
{
	bool operator() (dk::UnitTest* pUT, unsigned int ui) const { return pUT->key().group() < ui; }
	bool operator() (unsigned int ui, dk::UnitTest* pUT) const { return ui < pUT->key().group(); }
};
// Comparison for std::sort.
static bool compareByTestNo(UnitTest* p1, UnitTest* p2)
{
	return p1->key().test() < p2->key().test();
}
// Comparison for std::sort.
static bool compareByKey(UnitTest* p1, UnitTest* p2)
{
	return p1->key() < p2->key();
}

void UnitTest::updateAndSort() noexcept
{
	auto& list = singleton();

	// Get the user defined keys and descriptions.
	for (auto& pTst : list)
		pTst->update();

	// First sort UTs by group. At this stage we don't need a more granular
	// sort order because all we want to achieve is to be in a position to
	// update the default UT numbers (0) with unique nonzero numbers. Once
	// this objective has been achieved we will then sort the list at the
	// more granular level including the UT numbers as well.
	std::sort(list.begin(), list.end(), compareByGroupNo);

	// Identify unit tests that have the unit test number set to the default
	// (0) value and update them with unique nonzero numbers.
	std::vector<unsigned int> uniqUnitTestNumbers;
	auto iterStart = list.begin();
	while(iterStart != list.end())
	{
		// Identify a segment of the list where all unit tests belong to the same group.
		// This is possible because the list has already been sorted by Group No.
		unsigned int uiGroup{ (*iterStart)->key().group() };
		auto groupSegment = std::equal_range(iterStart, list.end(), uiGroup, ComparisonPredicate{});

		// Get unique unit test numbers that have already been used in the group.
		uniqUnitTestNumbers.clear();
		for (auto it = groupSegment.first; it < groupSegment.second; it++)
			uniqUnitTestNumbers.push_back( (*it)->key().test() );
		std::sort(uniqUnitTestNumbers.begin(), uniqUnitTestNumbers.end());
		auto it = std::unique(uniqUnitTestNumbers.begin(), uniqUnitTestNumbers.end());
		uniqUnitTestNumbers.resize(std::distance(uniqUnitTestNumbers.begin(), it));

		// Update unit tests of the group that have the default test number (0) assigned.
		unsigned int uiCurrentTestNo { 0 };
		unsigned int uiGreatestTestNo{ 0 };
		unsigned int uiNewTestNo{ 1 };
		for (auto it = groupSegment.first; it < groupSegment.second; it++)
		{	
			uiCurrentTestNo = (*it)->key().test();
			if (uiGreatestTestNo < uiCurrentTestNo)
				uiGreatestTestNo = uiCurrentTestNo;

			if (uiCurrentTestNo == 0)
			{
				// Find a suitable new test number.
				while (std::binary_search(uniqUnitTestNumbers.begin(), uniqUnitTestNumbers.end(), uiNewTestNo))
					uiNewTestNo++;
				// Assign the new test number to the key.
				(*it)->key().test(uiNewTestNo);
				// Get ready to process the next default number.
				uiNewTestNo++;
			}
			else
			{
				// Make sure the autonumbers always exceed the greatest user
				// defined number encountered by now. Not that it matters much,
				// but it's good to see some order in the ultimate sequence.
				if (uiNewTestNo < uiGreatestTestNo)
					uiNewTestNo = uiGreatestTestNo + 1;
			}
		}

		// The default test numbers have been updated.
		// Let's now finalise the sort order within the segment.
		std::sort(groupSegment.first, groupSegment.second, compareByTestNo);

		iterStart = groupSegment.second;
	}
}
const UTList& UnitTest::list() noexcept
{
	return singleton();
}
UTKey& UnitTest::key() noexcept
{
	return utkCompositeKey_;
}
const std::string& UnitTest::description() const noexcept
{
	return description_;
}
void UnitTest::_update(unsigned int uiGroup, unsigned int uiTest, const std::string& description) noexcept
{
	utkCompositeKey_ = UTKey(uiGroup, uiTest);
	if (description.size() == 0)
		description_ = "No user defined description";
	else
		description_ = description;
}
void UnitTest::_update(unsigned int uiGroup, const std::string& description) noexcept
{
	_update(uiGroup, 0, description);
}
void UnitTest::_update(const std::string& description) noexcept
{
	_update(0, 0, description);
}
