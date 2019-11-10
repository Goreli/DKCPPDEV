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
#include "dk_utf.hpp"

void dk::verify(UTGListVerifier& utgVerifier, UTListVerifier& utVerifier) noexcept
{
	// Prepare the global lists.
	UnitTestGroup::sort();
	for (auto& pTst : UnitTest::list()) {
		pTst->describe();
		pTst->initCompositeKey();
	}
	UnitTest::sort();

	utgVerifier.verify(UnitTestGroup::list(), UnitTest::list());
	utVerifier.verify(UnitTest::list(), utgVerifier.instanceCounter());
}

static void printSummaryOfCounts(size_t uiUnitTestCount, size_t uiGroupCount, size_t uiFailedUTCount)
{
	size_t uiPassedUTCount = uiUnitTestCount - uiFailedUTCount;

	// Print the summary of counts.
	std::cout << "Unit tests: " << uiUnitTestCount << ". Groups: " << uiGroupCount << "." << std::endl;
	if (uiFailedUTCount)
		// Print in red color.
		std::cout << "\033[3;41;37m" << "Failed: " << uiFailedUTCount << "." << "\033[0m" << " Passed: " << uiPassedUTCount << "." << std::endl;
	else
		// Print in green color.
		std::cout << "\033[3;42;30m" << "Failed: " << uiFailedUTCount << ". Passed: " << uiPassedUTCount << "." << "\033[0m" << std::endl;
}

static void printSummaryOfFailedTests(const dk::UTList& utFailedUTList)
{
	if (utFailedUTList.size()) {
		// Print in red color.
		std::cout << "\033[3;41;37m" << "Summary of failed unit tests:" << "\033[0m" << std::endl;

		for (auto pUT : utFailedUTList) {
			size_t uiGroupKey = pUT->getKey().groupKey();
			size_t uiTestKey = pUT->getKey().testKey();
			std::cout << "\033[3;41;37m" << uiGroupKey << "." << uiTestKey << "\033[0m" << ": " << pUT->getDescription() << std::endl;
		}
		std::cout << std::endl;
	}
}

static void printAndExecuteUT(dk::UnitTest* pUnitTest, dk::UTList& utFailedUTList)
{
	size_t uiGroupKey = pUnitTest->getKey().groupKey();
	size_t uiTestKey = pUnitTest->getKey().testKey();

	// Print and execute the unit test.
	std::cout << "\tUnit test " << uiGroupKey << "." << uiTestKey << ": " << pUnitTest->getDescription() << std::endl;
	if (pUnitTest->execute()) {
		// Print in green color.
		std::cout << "\t\t" << "\033[3;42;30m" << "Passed" << "\033[0m" << std::endl;
	}
	else {
		utFailedUTList.push_back(pUnitTest);
		// Print in red color.
		std::cout << "\t\t" << "\033[3;41;37m" << "Failed" << "\033[0m" << std::endl;
	}
}

void dk::run(const UTGList& utgList, const UTList& utList) noexcept
{
	UTList utFailedUTList;
	size_t uiGroupCount{ 0 };
	size_t uiUnitTestCount{ 0 };

	// Print unique linked groups and execute respective unique linked unit tests.
	size_t uiLastGroupKey = 0;
	for (auto& pUnitTest : utList) {
		// Ignore zero group key unit tests.
		size_t uiGroupKey = pUnitTest->getKey().groupKey();
		if (uiGroupKey == 0)
			continue;

		// Ignore zero key unit tests.
		size_t uiTestKey = pUnitTest->getKey().testKey();
		if (uiTestKey == 0)
			continue;

		// Check if the group has changed.
		if (uiLastGroupKey != uiGroupKey) {
			// Find the record of the group and print it.
			for (auto& pGroup : utgList)
				if (pGroup->getKey() == uiGroupKey) {
					if(uiGroupCount > 0)
						std::cout << std::endl;
					std::cout << "Group " << pGroup->getKey() << ": " << pGroup->getDescription() << std::endl;
					break;
				}
			uiLastGroupKey = uiGroupKey;
			uiGroupCount++;
		}

		printAndExecuteUT(pUnitTest, utFailedUTList);
		uiUnitTestCount++;
	}
	std::cout << std::endl;

	printSummaryOfFailedTests(utFailedUTList);
	printSummaryOfCounts(uiUnitTestCount, uiGroupCount, utFailedUTList.size());
}
