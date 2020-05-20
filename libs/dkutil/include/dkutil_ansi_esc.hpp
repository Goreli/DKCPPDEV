/* dkutil_ansi_esc.hpp
Declares ANSI escape sequence related structures and routines.

Usefule ANSI ESC links:
	https://en.wikipedia.org/wiki/ANSI_escape_code
	https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/

Copyright(c) 2020 David Krikheli

Modification history:
    20/May/2020 - David Krikheli created the module.
*/

#ifndef dkutil_ansi_esc_hpp
#define dkutil_ansi_esc_hpp

namespace dk {
	// This function is primarily required on the Windows 10 platform
	// to set up Windows Console so it can handle ANSI Esc sequences.
	//
	// On Linux the terminal is expected to be ANSI Seq capable nowadays
	// and the function is not required.
	//
	// The implementation of the function utilises conditional compilation
	// and only compiles its code (Windows Console API based) if the _CONSOLE
	// preprocessor symbol is defined. If this symbol is not defined then no
	// code is compiled inside the function and it only acts as an empty
	// placeholder for cross-platform portability purposes.
	void enableANSIEscapeSequences();
};	// namespace dk
#endif //	dkutil_ansi_esc_hpp
