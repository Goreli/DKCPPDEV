/* cli_parser_base.cpp
Defines a base class for command line parsers.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#include <sstream>
#include <locale>
#include "dkutil_cli_parser_base.hpp"

using namespace std;
using namespace dk;

size_t CLIParserBase::_stringto_size_t(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	size_t iNum{ 0 };
	iss >> iNum;
	return iNum;
}
double CLIParserBase::_stringto_double(const string& strNum) noexcept {
	std::istringstream iss(strNum);
	double dNum{ 0.0 };
	iss >> dNum;
	return dNum;
}

CLIParserException::CLIParserException(const string& strWhat)
: invalid_argument{strWhat} {
}
CLIParserBase::CLIParserBase(int argc, char** argv)
	: _argc{ argc }, _argv{ argv }, _inxArg{0}
{
}
CLIParserBase::~CLIParserBase()
{
}
void CLIParserBase::_advanceAndCheckMissingValue() {
	char* pOption = _argv[_inxArg];
	_inxArg++;
	if (_inxArg == _argc)
		throw CLIParserException(string("Missing value in CLI option ") + pOption + '.');
}
bool CLIParserBase::_uintOption(const string& strOption, size_t& iValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();

		if (!isdigit(_argv[_inxArg][0]))
			throw CLIParserException(string("Require numeric argument in CLI option -") + strOption + '.');

		iValue = CLIParserBase::_stringto_size_t(_argv[_inxArg]);
		if (iValue == 0)
			throw CLIParserException(string("Require non-zero argument in CLI option -") + strOption + '.');

		return true;
	}
	return false;
}
bool CLIParserBase::_doubleOption(const string& strOption, double& dValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();

		if (!isdigit(_argv[_inxArg][0]))
			throw CLIParserException(string("Require numeric argument in CLI option -") + strOption + '.');

		dValue = CLIParserBase::_stringto_double(_argv[_inxArg]);
		if (dValue == 0.0)
			throw CLIParserException(string("Require non-zero argument in CLI option -") + strOption + '.');

		return true;
	}
	return false;
}
bool CLIParserBase::_boolOption(const string& strOption, bool& bValue) noexcept {
	if (string(_argv[_inxArg]+1) == strOption) {
		bValue = true;
		return true;
	}
	return false;
}
bool CLIParserBase::_strOption(const string& strOption, string& strValue) {
	if (string(_argv[_inxArg]+1) == strOption) {
		_advanceAndCheckMissingValue();
		if (strValue.size() > 0)
			throw CLIParserException(string("Redefinition of string value in CLI option -") + strOption + '.');
		strValue = string(_argv[_inxArg]);
		return true;
	}
	return false;
}
bool CLIParserBase::_threeStateOption(const string& strOption, bool& bOption, const string& strVal1, const string& strVal2, bool& bVal) {
	if (_boolOption(strOption, bOption)) {
		_advanceAndCheckMissingValue();
		if (string(_argv[_inxArg]+0) != strVal1 && string(_argv[_inxArg]+0) != strVal2)
			throw CLIParserException(string("Invalid value in CLI option -") + strOption + '.');
		bVal = (string(_argv[_inxArg]+0) == strVal1) ? true : false;
		return true;
	}
	return false;
}
