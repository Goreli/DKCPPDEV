/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#include "dk_cdt.hpp"

using namespace dk;

size_t ConstDestTracker::iConstructorCount_{ 0 };
size_t ConstDestTracker::iDestructorCount_{ 0 };
size_t ConstDestTracker::iControlSum_{ 0 };

ConstDestTracker::ConstDestTracker() noexcept
	: iObjSeqId_{ 0 }
{
	init_();
}
ConstDestTracker::ConstDestTracker(const ConstDestTracker&) noexcept
	: iObjSeqId_{ 0 }
{
	init_();
}
ConstDestTracker::ConstDestTracker(ConstDestTracker&&) noexcept
	: iObjSeqId_{ 0 }
{
	init_();
}

void ConstDestTracker::init_() noexcept
{
	iConstructorCount_++;
	iObjSeqId_ = iConstructorCount_;
	iControlSum_ += iObjSeqId_;
}

ConstDestTracker::~ConstDestTracker() noexcept
{
	iDestructorCount_++;
	iControlSum_ -= iObjSeqId_;
}

ConstDestTracker& ConstDestTracker::operator = (const ConstDestTracker&) noexcept
{
	return *this; 
}

ConstDestTracker& ConstDestTracker::operator = (ConstDestTracker&&)  noexcept
{
	return *this;
}

size_t ConstDestTracker::getConstCount(void) noexcept
{ return iConstructorCount_; }

size_t ConstDestTracker::getDestCount(void) noexcept
{ return iDestructorCount_; }

size_t ConstDestTracker::getCtrlSum(void) noexcept
{ return iControlSum_; }

void ConstDestTracker::reset() noexcept
{ iConstructorCount_ = iDestructorCount_ = iControlSum_ = 0; }
