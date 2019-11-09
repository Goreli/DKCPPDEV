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

#ifndef libs_dk_utv_hpp
#define libs_dk_utv_hpp

#include <map>
#include "dk_utgv.hpp"

namespace dk {
	typedef std::map<UTKey, unsigned int> UTInstanceCounter;

	class UTListVerifier {
	public:
		// The lists passed into this function can be either sorted or unsorted.
		// Prefer sorted group list as it will produce a better structured
		// verification report.
		void verify(const UTList&, const UTGInstanceCounter&) noexcept;

		const UTInstanceCounter& instanceCounter() const noexcept;
		const UTList& zeroGroupList() const noexcept;
		const UTList& duplicateLinkedUnitTestList() const noexcept;
		const UTList& duplicateUnlinkedUnitTestList() const noexcept;
		const UTList& uniqueLinkedUnitTestList() const noexcept;
		const UTList& uniqueUnlinkedUnitTestList() const noexcept;

	private:
		void countInstances_(const UTList&) noexcept;
		void populateLists_(const UTList&, const UTGInstanceCounter&) noexcept;

		// In this map container the key is the Composite Key and the value is the
		// count of unit tests with that Composite Key. In other words if there are
		// custom defined unit tests with duplicate Composite Key then the respective
		// value in the instance counter object will be greater than 1. For each
		// correct unit test (with no duplicate) the value will equal 1.
		UTInstanceCounter utInstanceCounter_;

		// A list of unit tests whose Group Key is set to a 0 (zero).
		UTList utZeroGroupList_;

		// A list of unit tests with duplicate Composite Key and associated groups
		// linked to them.
		UTList utDuplicateLinkedUnitTestList_;

		// A list of unit tests with duplicate Composite Key and no associated
		// groups linked to them.
		UTList utDuplicateUnlinkedUnitTestList_;

		// A list of unit tests each having a unique Composite Key and associated
		// groups linked to them.
		UTList utUniqueLinkedUnitTestList_;

		// A list of unit tests each having a unique Composite Key and no
		// associated groups linked to them.
		UTList utUniqueUnlinkedUnitTestList_;
	};

}	// namespace dk

#endif	// libs_dk_utv_hpp

