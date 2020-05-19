/* cli_misc.hpp
Declares miscellaneous CLI routines.

Usefule ANSI ESC links:
	https://en.wikipedia.org/wiki/ANSI_escape_code
	https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/

Copyright(c) 2019 David Krikheli

Modification history:
    10/May/2020 - David Krikheli created the module.
*/

#ifndef dkutil_cli_misc_hpp
#define dkutil_cli_misc_hpp

namespace dk {
	void enableEscapeSequences();
	void forceThousandsSeparators(std::ostream& os);
};	// namespace dk
#endif //	dkutil_cli_misc_hpp
