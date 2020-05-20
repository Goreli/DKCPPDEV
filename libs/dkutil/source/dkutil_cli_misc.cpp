/* dkutil_cli_misc.cpp
Defines miscellaneous CLI routines.

Copyright(c) 2020 David Krikheli

Modification history:
    10/May/2020 - David Krikheli created the module.
*/

#include <sstream>
#include <iostream>
#include <locale>
#include "dkutil_cli_misc.hpp"

using namespace std;

// Make sure integrals are printed with thousands separators included.
struct separate_thousands : std::numpunct<char> {
	char_type do_thousands_sep() const override { return ','; }  // separate with commas
	string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};
void dk::forceThousandsSeparators(std::ostream& os) {
	auto thousands = std::make_unique<separate_thousands>();
	os.imbue(std::locale(std::cout.getloc(), thousands.release()));
}
