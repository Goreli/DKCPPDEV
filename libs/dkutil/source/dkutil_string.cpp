/* dkutil_string.cpp
Declares miscellaneous string utility routines.

Copyright(c) 2020 David Krikheli

Modification history:
	13/May/2020 - David Krikheli created the module.
*/

#include "dkutil_string.hpp"

using namespace std;
using namespace dk;

std::string& dk::ltrim(std::string& str, const std::string& chars)
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}
std::string& dk::rtrim(std::string& str, const std::string& chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}
std::string& dk::trim(std::string& str, const std::string& chars)
{
	return dk::ltrim(dk::rtrim(str, chars), chars);
}
