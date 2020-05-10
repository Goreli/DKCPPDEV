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

#ifndef libs_dk_cdt_hpp
#define libs_dk_cdt_hpp

namespace dk {
	class ConstDestTracker {
		// Make an object of this class a member of a class that you want to test.
		// Access type doesn't have any significance - should probably be made private.
		// Call reset() before the start of each test.
		//
		// The following is expected at the end of the test:
		// 1. getConstCount() == getDestCount()
		// 2. getCtrlSum() == 0

	public:
		ConstDestTracker() noexcept;

		// The copy constructor has only been introduced to
		// preclude the default compiler generated construction.
		// This is because we need to protect iObjSeqId_ from being
		// corrupted.
		ConstDestTracker(const ConstDestTracker&) noexcept;
		// The move constructor serves a similar purpose - protect
		// iObjSeqId_ from corruption in case a wrapper class applies
		// brute force and tries to overwrite the content.
		ConstDestTracker(ConstDestTracker&&) noexcept;

		virtual ~ConstDestTracker() noexcept;

		// The copy assignment operator below has only been introduced
		// to preclude the default compiler generated assignment.
		// This is because we need to protect iObjSeqId_ from being
		// corrupted.
		ConstDestTracker& operator = (const ConstDestTracker&) noexcept;
		// The move assignment operator serves a similar purpose - protect
		// iObjSeqId_ from corruption in case a wrapper class applies
		// brute force and tries to overwrite the content.
		ConstDestTracker& operator = (ConstDestTracker&&) noexcept;

		static size_t getConstCount(void) noexcept;
		static size_t getDestCount(void) noexcept;
		static size_t getCtrlSum(void) noexcept;
		static void   reset() noexcept;

	private:
		// Total count of instrumented objects created.
		static size_t iConstructorCount_;	

		// Total count of instrumented objects destroyed.
		static size_t iDestructorCount_;

		// Increases as instrumented objects are created.
		// Decreases as instrumented objects are destroyed.
		static size_t iControlSum_;

		// Unique id of an instrumented object.
		// If the object is overwritten by a brute force of a bug of some sort
		// then correctness of the iControlSum_ calculation (reduction) will be
		// impacted and we are likely to witness a non-zero value of the control
		// sum at the end of the test. Otherwise, if there is no brute force/bug
		// distorting the value then we should see the following at the end of the
		// test: iControlSum_ == 0.
		size_t iObjSeqId_;

		void init_() noexcept;
	};
}	// namespace dk

#endif	// libs_dk_cdt_hpp

