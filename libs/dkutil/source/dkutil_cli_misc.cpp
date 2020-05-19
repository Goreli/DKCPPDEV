/* dkutil_cli_misc.cpp
Declares miscellaneous CLI routines.

Copyright(c) 2020 David Krikheli

Modification history:
    10/May/2020 - David Krikheli created the module.
*/

#ifdef _CONSOLE
// Enable ANSI escape sequences on Windows 10.
// Assuming Linux doesn't require this type of enablement nowadays.
#include <windows.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008
#endif

#include <sstream>
#include <iostream>
#include <locale>
#include "dkutil_cli_misc.hpp"

using namespace std;

void dk::enableEscapeSequences() {
#ifdef _CONSOLE
	// If this is Windows 10 then use a singleton pattern to execute
	// the ANSI escape sequence enablement code only once.
	static HANDLE hStdout{ nullptr };
	if (!hStdout) {
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD l_mode;
		GetConsoleMode(hStdout, &l_mode);
		SetConsoleMode(hStdout, l_mode |
			ENABLE_VIRTUAL_TERMINAL_PROCESSING |
			DISABLE_NEWLINE_AUTO_RETURN);
	}
#endif
}

// Make sure integrals are printed with thousands separators included.
struct separate_thousands : std::numpunct<char> {
	char_type do_thousands_sep() const override { return ','; }  // separate with commas
	string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};
void dk::forceThousandsSeparators(std::ostream& os) {
	auto thousands = std::make_unique<separate_thousands>();
	os.imbue(std::locale(std::cout.getloc(), thousands.release()));
}
