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

#ifndef libs_dk_utgv_hpp
#define libs_dk_utgv_hpp

#include <map>
#include "dkutf_utg.hpp"
#include "dkutf_ut.hpp"

namespace dk {
	typedef std::map<unsigned int, unsigned int> UTGInstanceCounter;

	class UTGListVerifier {
	public:
		// The lists passed into this function can be either sorted or unsorted.
		// Prefer sorted group list as it will produce a better structured
		// verification report.
		void verify(const UTGList&, const UTList&) noexcept;

		const UTGInstanceCounter& instanceCounter() const noexcept;
		const UTGList& zeroGroupList() const noexcept;
		const UTGList& duplicateLinkedGroupList() const noexcept;
		const UTGList& duplicateUnlinkedGroupList() const noexcept;
		const UTGList& uniqueLinkedGroupList() const noexcept;
		const UTGList& uniqueUnlinkedGroupList() const noexcept;

	private:
		void countInstances_(const UTGList&) noexcept;
		void populateLists_(const UTGList&, const UTList&) noexcept;


		// In this map container the key is the Group Key and the value is the count
		// of groups with that Group Key. In other words if there are custom defined
		// groups with duplicate Group Key then the respective value in the instance
		// counter object will be greater than 1. For each correct custom group (with
		// no duplicate) the value will equal 1.
		UTGInstanceCounter utgInstanceCounter_;

		// A list of custom defined groups whose Group Key is set to a 0 (zero).
		UTGList utgZeroGroupList_;

		// A list of custom defined groups with duplicate Group Key that have
		// associated unit tests linked to them.
		UTGList utgDuplicateLinkedGroupList_;

		// A list of custom defined groups with duplicate Group Key and no
		// associated unit tests linked to them.
		UTGList utgDuplicateUnlinkedGroupList_;

		// A list of custom defined groups each having a unique Group Key
		// and associated unit tests linked to them.
		UTGList utgUniqueLinkedGroupList_;

		// A list of custom defined groups each having a unique Group Key
		// and no associated unit tests linked to them.
		UTGList utgUniqueUnlinkedGroupList_;
	};

}	// namespace dk

#endif	// libs_dk_utgv_hpp

