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

#ifndef libs_dk_ut_hpp
#define libs_dk_ut_hpp

#include <string>
#include <vector>
#include "dkutf_utk.hpp"

namespace dk {
	class UnitTest;
	typedef std::vector<UnitTest*> UTList;

	class UnitTest {
	public:
		UnitTest();
		virtual ~UnitTest();

		static void sort() noexcept;
		static const UTList& list() noexcept;

		// Expect a nonabstract override to be defined in the application module
		// in order to ascertain _uGroupKey, _uTestKey and _sDescription.
		//
		// In theory this function should be declared as noexcept, except we want to
		// minimise the amount of scripting in the application module. The motivation
		// here is to make the process of defining application unit tests as user
		// friendly as possible. Risks seem to be minimal in this context.
		virtual void describe() = 0;

		// Set the composite key once the application module has defined
		// a nonabstract override of the identify() member function.
		void initCompositeKey() noexcept;

		// Expect a nonabstract override to be defined in the application module
		// in order to execute the actual unit test.
		virtual bool execute() = 0;

		const UTKey& getKey() const noexcept;
		const std::string& getDescription() const noexcept;

	protected:
		// The following three data members have been made noncompliant
		// with the standard naming convention because they are part of
		// the application level user interface. These names have been
		// chosen to provide a "clean" well understood interface to the
		// human user.
		std::string description;
		unsigned test;
		unsigned group;

	private:
		UTKey utkCompositeKey_;

	};
}	// namespace dk

#endif	// libs_dk_ut_hpp
