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

#include "dkutf_utgv.hpp"

using namespace dk;

void UTGListVerifier::verify(const UTGList& utgList, const UTList& utList) noexcept
{
	countInstances_(utgList);
	populateLists_(utgList, utList);
}

void UTGListVerifier::countInstances_(const UTGList& utgList) noexcept
{
	// Count instances of groups.
	for (auto& pGroup : utgList) {
		unsigned int uiGroup = pGroup->group();
		try {
			unsigned int uiValue = utgInstanceCounter_.at(uiGroup);
			utgInstanceCounter_[uiGroup] = uiValue + 1;
		}
		catch (...) {
			utgInstanceCounter_[uiGroup] = 1;
		}
	}
}

void UTGListVerifier::populateLists_(const UTGList& utgList, const UTList& utList) noexcept
{
	for (auto& pGroup : utgList) {
		unsigned int uiGroupKey = pGroup->group();

		bool bHasDuplicate = (utgInstanceCounter_[uiGroupKey] > 1);
		bool bIsLinked = false;
		for (auto& pTest : utList) 
			if (pTest->key().group() == uiGroupKey) {
				bIsLinked = true;
				break;
			}

		if (bHasDuplicate) {
			if (bIsLinked)
				utgDuplicateLinkedGroupList_.push_back(pGroup);
			else
				utgDuplicateUnlinkedGroupList_.push_back(pGroup);
		} else {
			if (bIsLinked)
				utgUniqueLinkedGroupList_.push_back(pGroup);
			else
				utgUniqueUnlinkedGroupList_.push_back(pGroup);
		}
	}
}

const UTGInstanceCounter& UTGListVerifier::instanceCounter() const noexcept {
	return utgInstanceCounter_;
}
const UTGList& UTGListVerifier::duplicateLinkedGroupList() const noexcept {
	return utgDuplicateLinkedGroupList_;
}
const UTGList& UTGListVerifier::duplicateUnlinkedGroupList() const noexcept {
	return utgDuplicateUnlinkedGroupList_;
}
const UTGList& UTGListVerifier::uniqueLinkedGroupList() const noexcept {
	return utgUniqueLinkedGroupList_;
}
const UTGList& UTGListVerifier::uniqueUnlinkedGroupList() const noexcept {
	return utgUniqueUnlinkedGroupList_;
}
