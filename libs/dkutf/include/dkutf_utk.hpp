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

#ifndef libs_dkutf_utk_hpp
#define libs_dkutf_utk_hpp

#include <string>

namespace dk {

	class UTKey{
	public:
		UTKey(unsigned int, unsigned int);
		virtual ~UTKey();

		bool operator < (const UTKey&) const noexcept;
		bool operator == (const UTKey&) const noexcept;
		bool operator <= (const UTKey&) const noexcept;
		unsigned int group() const noexcept;
		unsigned int test() const noexcept;
		void test(unsigned int) noexcept;
		std::string str() const noexcept;
		bool autoNumberedTestNo() const noexcept;

	private:
		unsigned int uiGroup_;
		unsigned int uiTest_;
		bool bAutoNumberedTestNo_;
	};

}	// namespace dk

#endif	// libs_dkutf_utk_hpp
