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

#ifndef libs_dk_utg_hpp
#define libs_dk_utg_hpp

#include <string>
#include <vector>

namespace dk {

	class BasePositiveInteger {
	public:
		virtual operator int() const noexcept = 0;
	};

	template <int i>
	class PositiveInteger : public BasePositiveInteger {
	public:
		PositiveInteger():i_(i) {
			static_assert(i > 0, "PositiveInteger not positive");
		}
		virtual operator int() const noexcept {
			return i_;
		}
	private:
		int i_;
	};

	class UnitTestGroup;
	typedef std::vector<UnitTestGroup*> UTGList;

	class UnitTestGroup {
	public:
		// This is a public constructor required to create user defined groups
		// of unit tests. User defined groups must have their group ids set to
		// positive values. Hence the use of the BasePositiveInteger type.
		// Also, see the private constructor declared in this class required
		// to define a system default group with the group id set to a 0.
		UnitTestGroup(const BasePositiveInteger&, const std::string&);
		virtual ~UnitTestGroup();

		static const UTGList& list() noexcept;
		static void sort() noexcept;

		unsigned int group() const noexcept;
		const std::string& description() const noexcept;

	private:
		unsigned int uiGroup_;
		std::string sDescription_;

		// This is a private constructor required to create the system default
		// group with the group id set to a 0.
		UnitTestGroup();
		static UnitTestGroup objDefaultGroup_;
	};
}	// namespace dk

#endif	// libs_dk_utg_hpp
