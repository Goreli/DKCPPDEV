/* dkutil_ansi_esc.cpp
Declares ANSI escape sequence related structures and routines.

Usefule ANSI ESC links:
	https://en.wikipedia.org/wiki/ANSI_escape_code
	https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/

Copyright(c) 2020 David Krikheli

Modification history:
	20/May/2020 - David Krikheli created the module.
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
#include "dkutil_ansi_esc.hpp"

using namespace std;

void dk::enableANSIEscapeSequences() {
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
