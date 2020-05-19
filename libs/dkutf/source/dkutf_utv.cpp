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

#include "dkutf_utv.hpp"

using namespace dk;

void UTListVerifier::verify(const UTList& utList, const UTGInstanceCounter& utgInstanceCounter) noexcept
{
	countInstances_(utList);
	populateLists_(utList, utgInstanceCounter);
}

void UTListVerifier::countInstances_(const UTList& utList) noexcept
{
	// Count instances of unit tests.
	for (auto& pUnitTest : utList) {
		UTKey utKey = pUnitTest->key();
		try {
			unsigned int uiValue = utInstanceCounter_.at(utKey);
			utInstanceCounter_[utKey] = uiValue + 1;
		}
		catch (...) {
			utInstanceCounter_[utKey] = 1;
		}
	}
}

void UTListVerifier::populateLists_(const UTList& utList, const UTGInstanceCounter& utgInstanceCounter) noexcept
{
	for (auto& pUnitTest : utList) {
		UTKey utKey = pUnitTest->key();

		unsigned int uiGroup = utKey.group();

		bool bHasDuplicate = (utInstanceCounter_[utKey] > 1);
		bool bIsLinked = (utgInstanceCounter.count(uiGroup) > 0);

		if (bHasDuplicate) {
			if (bIsLinked)
				utDuplicateLinkedUnitTestList_.push_back(pUnitTest);
			else
				utDuplicateUnlinkedUnitTestList_.push_back(pUnitTest);
		} else {
			if (bIsLinked)
				utUniqueLinkedUnitTestList_.push_back(pUnitTest);
			else
				utUniqueUnlinkedUnitTestList_.push_back(pUnitTest);
		}
	}
}

const UTInstanceCounter& UTListVerifier::instanceCounter() const noexcept {
	return utInstanceCounter_;
}
const UTList& UTListVerifier::duplicateLinkedUnitTestList() const noexcept {
	return utDuplicateLinkedUnitTestList_;
}
const UTList& UTListVerifier::duplicateUnlinkedUnitTestList() const noexcept {
	return utDuplicateUnlinkedUnitTestList_;
}
const UTList& UTListVerifier::uniqueLinkedUnitTestList() const noexcept {
	return utUniqueLinkedUnitTestList_;
}
const UTList& UTListVerifier::uniqueUnlinkedUnitTestList() const noexcept {
	return utUniqueUnlinkedUnitTestList_;
}
