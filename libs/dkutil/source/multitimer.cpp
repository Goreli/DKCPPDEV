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

#include <fstream>
#include "multitimer.hpp"

using namespace dk;

MultiTimer::MultiTimer(size_t size)
   : iCycleCounter_{ 0 }
{
   durations_.resize(size);
}
MultiTimer::~MultiTimer()
{
}
void MultiTimer::start() noexcept
{
   timeStamp_ = std::chrono::high_resolution_clock::now();
   iCycleCounter_++;
}
void MultiTimer::check(size_t inx)
{
   auto tempTimeStamp = std::chrono::high_resolution_clock::now();
   durations_[inx] += tempTimeStamp - timeStamp_;
   timeStamp_ = tempTimeStamp;
}
void MultiTimer::save(const std::string strFile)
{
   std::ofstream ofs;
   // Throw an exception should anything go wrong.
   ofs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   ofs.open(strFile, std::ofstream::out | std::ofstream::app);

   ofs << "Cycles: " << iCycleCounter_ << std::endl;
   size_t inx{ 0 };
   for (auto& dur : durations_)
      ofs << '\t' << "Average duration for checkpoint " << inx++ << ": " << static_cast<double>(iCycleCounter_?dur.count()/iCycleCounter_:0.0) << std::endl;
   ofs << std::endl;

   ofs.close();
}
