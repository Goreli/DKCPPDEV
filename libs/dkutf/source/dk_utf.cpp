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

void dk::run(const UTGList& utgList, const UTList& utList) noexcept
{
	// Print unique linked groups and execute respective unique linked unit tests.
	unsigned int uiLastGroupKey = 0;
	for (auto& pUnitTest : utList) {
		unsigned int uiGroupKey = pUnitTest->getKey().groupKey();

		// Print the group if it's just changed.
		if (uiLastGroupKey != uiGroupKey) {
			uiLastGroupKey = uiGroupKey;
			for (auto& pGroup : utgList)
				if (pGroup->getKey() == uiGroupKey) {
					std::cout << std::endl;
					std::cout << "Group " << pGroup->getKey() << ": " << pGroup->getDescription() << std::endl;
					break;
				}
		}

		// Print and execute the unit test.
		std::cout << "\tUnit test " << pUnitTest->getKey().groupKey() << "." << pUnitTest->getKey().testKey() << ": " << pUnitTest->getDescription() << std::endl;
		if (pUnitTest->execute())
			std::cout << "\t\t" << "\033[3;42;30m" << "Passed" << "\033[0m" << std::endl;
		else
			std::cout << "\t\t" << "\033[3;41;37m" << "Failed" << "\033[0m" << std::endl;
	}
}
