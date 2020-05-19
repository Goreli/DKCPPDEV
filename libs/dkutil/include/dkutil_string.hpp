/* dkutil_string.hpp
Declares miscellaneous string utility routines.

Copyright(c) 2020 David Krikheli

Modification history:
    13/May/2020 - David Krikheli created the module.
*/

#ifndef dkutil_string_hpp
#define dkutil_string_hpp

#include <string>

namespace dk {
	std::string& ltrim(std::string&, const std::string& = "\t\n\v\f\r ");
	std::string& rtrim(std::string&, const std::string& = "\t\n\v\f\r ");
	std::string&  trim(std::string&, const std::string& = "\t\n\v\f\r ");
};	// namespace dk
#endif //	dkutil_string_hpp
