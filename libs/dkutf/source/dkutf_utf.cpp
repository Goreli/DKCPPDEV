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
#include "dkutf_utf.hpp"

void dk::updateAndSort() noexcept
{
	// Prepare the global lists.
	UnitTestGroup::sort();
	UnitTest::updateAndSort();
}

void dk::verifyUT(UTGListVerifier& utgVerifier, UTListVerifier& utVerifier) noexcept
{
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
		std::cout << "\033[3;41;37m" << "Summary of failed unit tests:" << "\033[0m" << "\n";

		for (auto pUT : utFailedUTList)
			std::cout << "\033[3;41;37m" << pUT->key().str() << "\033[0m" << ": " << pUT->description() << "\n";

		std::cout << std::endl;
	}
}

static void printAndRunUT(dk::UnitTest* pUT, dk::UTList& utFailedUTList)
{
	// Print the unit test.
	std::string strUTKey = pUT->key().str();
	std::cout << "\t" << strUTKey << "\t";
	if(pUT->key().autoNumberedTestNo())
		std::cout << "[autonumbered] ";
	std::cout  << pUT->description() << std::endl;

	// Run the unit test.
	bool bPassed{false};
	try {
		bPassed = pUT->run();
	}
	catch (const std::exception& e)
	{
		bPassed = false;
		// Print in red color.
		std::cout << "\t\t" << strUTKey << "\t";
		std::cout << "\033[3;41;37m" << "caught unhandled exception of type \'";
		std::cout << typeid(e).name() << "\': " << e.what() << "\033[0m" << std::endl;
	}
	catch (...)
	{
		bPassed = false;
		// Print in red color.
		std::cout << "\t\t" << strUTKey << "\t";
		std::cout << "\033[3;41;37m" << "caught unhandled exception of unknown type.";
		std::cout << "\033[0m" << std::endl;
	}

	// Print the status.
	if (bPassed) {
		// Print in green color.
		std::cout << "\t\t" << strUTKey << "\t";
		std::cout << "\033[3;42;30m" << "passed" << "\033[0m" << std::endl;
	}
	else {
		utFailedUTList.push_back(pUT);
		// Print in red color.
		std::cout << "\t\t" << strUTKey << "\t";
		std::cout << "\033[3;41;37m" << "failed" << "\033[0m" << std::endl;
	}
}

void dk::runUT(const UTGList& utgList, const UTList& utList) noexcept
{
	UTList utFailedUTList;
	size_t uiGroupCount{ 0 };
	size_t uiUnitTestCount{ 0 };

	// Print unique linked groups and execute respective unique linked unit tests.
	size_t uiLastGroup { 0 };
	bool bNextGroupStarted{ true };

	for (auto& pUT : utList) {
		// Get the number of the group and check if the next group has started.
		size_t uiGroup = pUT->key().group();
		if (!bNextGroupStarted && uiGroup > uiLastGroup)
			bNextGroupStarted = true;

		// Check if we are dealing with the next group here.
		if (bNextGroupStarted) {
			if (uiGroupCount > 0)
				std::cout << std::endl;
			// Find the record of the group and print it.
			std::cout << utgList[uiGroupCount]->group() << ": " << utgList[uiGroupCount]->description() << std::endl;

			uiLastGroup = uiGroup;
			uiGroupCount++;
			bNextGroupStarted = false;
		}

		printAndRunUT(pUT, utFailedUTList);
		uiUnitTestCount++;
	}
	std::cout << std::endl;

	printSummaryOfFailedTests(utFailedUTList);
	printSummaryOfCounts(uiUnitTestCount, uiGroupCount, utFailedUTList.size());
}
