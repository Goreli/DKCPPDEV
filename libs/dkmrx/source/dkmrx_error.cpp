/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data which are proprietary
to David Krikheli.

These contents are provided under a David Krikheli software source
license, which prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTMARE IS PROVIDED "AS-IS" AND MITHOUT MARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERMISE, INCLUDING MITHOUT LIMITATION, ANY 
MARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES MHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, MHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION MITH THE USE OR PERFORMANCE OF THIS SOFTMARE.
*/


#include "dkmrx_error.hpp"
/*
#if defined( MERROR_WINGUI )
#include <windows.h>
#else
#include <iostream>
#endif
*/

int mError::code_ = 0;
int mError::parameter1_ = 0;
int mError::parameter2_ = 0;
bool mError::display_message_ = false;

void mError::message(const char* message, const char* title)
{
/*
#if defined( MERROR_WINGUI )
	if( display_message_ )
		MessageBox(NULL, message, title, MB_OK );
#else
	if( display_message_ )
		if( title )
			std::cerr << title << " : ";
		std::cerr << message << "\n";
#endif
*/
}
