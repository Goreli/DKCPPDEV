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

		static void updateAndSort() noexcept;
		static const UTList& list() noexcept;

		// Expect a nonabstract override to be defined in the application module
		// in order to ascertain description_ and utkCompositeKey_.
		virtual void update() noexcept = 0;

		// Expect a nonabstract override to be defined in the application module
		// in order to execute the actual unit test.
		// In general unit tests are expected to handle exceptions themselves.
		// If there is an unhandled exception thrown within the unit test code
		// it will be caught by the UTF framework and the unit test will be
		// reported as failed. This provides a mechanism for detecting "fantom"
		// exceptions that may have been thrown by ill documented libraries used
		// in unit tests.
		virtual bool run() = 0;

		const std::string& description() const noexcept;
		UTKey& key() noexcept;

	protected:
		void _update(unsigned int = 0, unsigned int = 0, const std::string & = {}) noexcept;
		void _update(unsigned int, const std::string &) noexcept;
		void _update(const std::string &) noexcept;

	private:
		std::string description_;
		UTKey utkCompositeKey_;

		// Identifies unit tests with the unit test numbers set to the default
		// (0) value and updates them to unique nonzero numbers.
		void updateDefault_(const UTList&);
	};
}	// namespace dk

#endif	// libs_dk_ut_hpp
