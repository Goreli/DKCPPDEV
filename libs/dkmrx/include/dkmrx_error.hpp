/*
  General purpose matrix library, Version 1.0
  Copyright (c) 1995 David Krikheli
 
  Permission to use, copy, modify, distribute, and sell this software and 
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notice and this permission notice appear in
  all copies of the software and related documentation, and (ii) the name of
  David Krikheli may not be used in any advertising or publicity relating to
  the software without the specific, prior written permission of David Krikheli.
  
  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 
  IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
  INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
  POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR 
  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef error_h
#define error_h

//#include "bool.hpp"

class mError
{
public:

static void set(int code = 0,int param1 = 0,int param2 = 0) { code_ = code; parameter1_ = param1; parameter2_ = param2; }
static int  get_code(void)                              { return code_; }
static int  get_param1(void)                            { return parameter1_; }
static int  get_param2(void)                            { return parameter2_; }
static void display_message(bool flag = true)       { display_message_ = flag; }
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

#endif // error_h
