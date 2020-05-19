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
#include "dkutf_utk_range.hpp"
#include "dkutil_string.hpp"

using namespace std;
using namespace dk;

UTKeyRangeException::UTKeyRangeException(const string& strWhat)
	: invalid_argument{ strWhat } {
}

UTKeyRange::UTKeyRange()
	: lowerBound_(1, 1), upperBound_(0, 0)
{}
UTKeyRange::~UTKeyRange()
{}
void UTKeyRange::parse(const std::string& str)
{
    stringstream ss(str);
    string token;

    getline(ss, token, '-');
    trim(token);
    lowerBound_ = parseUTKey_(token, UTKey(0, 0));

    getline(ss, token);
    trim(token);
    upperBound_ = parseUTKey_(token, lowerBound_);

    //unsigned int x = stoi(token);
}
bool UTKeyRange::operator()(const UTKey& key) const noexcept
{
	return (lowerBound_ <= key) && (key <= upperBound_);
}
