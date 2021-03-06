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

#include <sstream>
#include "dkutf_utk.hpp"

using namespace dk;

UTKey::UTKey(unsigned int uiGroup, unsigned int uiTest)
	: uiGroup_{ uiGroup }, uiTest_{ uiTest }, bAutoNumberedTestNo_{ false }
{
}
UTKey::~UTKey()
{}
bool UTKey::operator < (const UTKey& key) const noexcept
{
	if (uiGroup_ < key.uiGroup_)
		return true;
	if (uiGroup_ > key.uiGroup_)
		return false;
	if (uiTest_ < key.uiTest_)
		return true;
	return false;
}
bool UTKey::operator == (const UTKey& key) const noexcept
{
	return (uiGroup_ == key.uiGroup_) && (uiTest_ == key.uiTest_);
}
bool UTKey::operator <= (const UTKey& key) const noexcept
{
	return ((*this < key) || (*this == key));
}
unsigned int UTKey::group() const noexcept
{
	return uiGroup_;
}
unsigned int UTKey::test() const noexcept
{
	return uiTest_;
}
void UTKey::test(unsigned int uiTest) noexcept
{
	uiTest_ = uiTest;
	bAutoNumberedTestNo_ = true;
}
std::string UTKey::str() const noexcept
{
	std::ostringstream ss;
	ss << uiGroup_ << "." << uiTest_;
	return ss.str();
}
bool UTKey::autoNumberedTestNo() const noexcept
{
	return bAutoNumberedTestNo_;
}
