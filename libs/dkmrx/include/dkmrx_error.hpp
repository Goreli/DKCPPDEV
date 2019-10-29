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

#ifndef dkmrx_error_hpp
#define dkmrx_error_hpp
namespace dkmrx {

	class mError
	{
	public:

		static void set(int code = 0, int param1 = 0, int param2 = 0) { code_ = code; parameter1_ = param1; parameter2_ = param2; }
		static int  get_code(void) { return code_; }
		static int  get_param1(void) { return parameter1_; }
		static int  get_param2(void) { return parameter2_; }
		static void display_message(bool flag = true) { display_message_ = flag; }
		static void message(const char* message, const char* title = 0);

	private:

		static int      code_;
		static int      parameter1_;
		static int      parameter2_;
		static bool display_message_;
	};

#define MERR_INCOMPATIBLE_MATRICES              30001
#define MERR_INSUFFICIENT_MEMORY                30002
#define MERR_LVALUE                             30003
#define MERR_WRONG_ARGUMENT                     30004
#define MERR_WRONG_THIS_OBJECT                  30005
#define MERR_ILLEGAL_DIMENSION                  30006           
#define MERR_FILE_OPEN                          30007
#define MERR_FILE_READ                          30008
#define MERR_FILE_WRITE                         30009
#define MERR_UNEXPECTED_EOF                     30010
#define MERR_ZERO_LENGTH                        30011
#define MERR_INSUFFICIENT_MEMORY_OR_DISK        30012
#define MERR_VALUES_ALREADY_SAVED               30013
#define MERR_VALUES_NOT_LOADED                  30014
#define MERR_HEADER_READ                        30015
#define MERR_NO_SOLUTION                        30016
#define MERR_MANY_SOLUTIONS                     30017
#define MERR_NOT_IMPLEMENTED                    30018
#define MERR_GENERIC_ERROR                      30019

}	// namespace dkmrx
#endif // dkmrx_error_hpp
