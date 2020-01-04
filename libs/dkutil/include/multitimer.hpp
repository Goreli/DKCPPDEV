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

#ifndef dk_multitimer_hpp
#define dk_multitimer_hpp

#include <vector>
#include <chrono>
#include <string>

namespace dk {

   class MultiTimer {
   public:
      MultiTimer(size_t size);
      virtual ~MultiTimer();

      void start() noexcept;
      void check(size_t inx);
      void save(const std::string strFile);

   private:
      std::vector<std::chrono::duration<double>> durations_;
      std::chrono::time_point<std::chrono::steady_clock> timeStamp_;
      size_t iCycleCounter_;
   };

}  // namespace dk {

#endif // dk_multitimer_hpp
