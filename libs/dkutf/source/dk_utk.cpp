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

#include "dk_utk.hpp"

using namespace dk;

UTKey::UTKey(unsigned uGroupKey, unsigned uTestKey)
	: uGroupKey_{ uGroupKey }, uTestKey_{ uTestKey }
{}
UTKey::~UTKey()
{}
bool UTKey::operator < (const UTKey& key) const
{
	if (uGroupKey_ < key.uGroupKey_)
		return true;
	if (uGroupKey_ > key.uGroupKey_)
		return false;
	if (uTestKey_ < key.uTestKey_)
		return true;
	return false;
}
bool UTKey::operator == (const UTKey& key) const
{
	return (uGroupKey_ == key.uGroupKey_) && (uTestKey_ == key.uTestKey_);
}
unsigned UTKey::groupKey() const noexcept
{
	return uGroupKey_;
}
unsigned UTKey::testKey() const noexcept
{
	return uTestKey_;
}