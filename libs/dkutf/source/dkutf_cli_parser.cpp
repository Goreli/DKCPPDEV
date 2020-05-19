/* dkutf_cli_parser.cpp
Defines the command line parser of the Unit Testing Framework library.

Copyright(c) 2020 David Krikheli

Modification history:
	13/May/2020 - David Krikheli created the module.
*/

#include "dkutf_cli_parser.hpp"

using namespace std;
using namespace dk;

UTFCLIParser::UTFCLIParser(int argc, char** argv)
	: CLIParserBase(argc, argv)
{}

void UTFCLIParser::parse() {
	if (_argc > 7)
		throw CLIParserException(string("Too many CLI options: ") + to_string(_argc));

	for (_inxArg = 1; _inxArg < _argc; _inxArg++) {
		string strOption = string(_argv[_inxArg]);

		if (strOption[0] == '-') {
		}
		string strErrMsg = "CLI options should start with \'-\'. Invalid argument #";
		strErrMsg += to_string(_inxArg + 1) + ": " + strOption + ".";
		throw CLIParserException(strErrMsg);
	}	// for
}

void UTFCLIParser::printUsage() noexcept {
	cout << "Runs a suit of DKUTF unit tests and displays information about it." << '\n';

	cout << "DKUTF. Unit Testing Framework v1.0. Copyright (c) 2019 David Krikheli." << '\n';
	cout << "Refer the following link for comprehensive help information:" << '\n';
	cout << " " << "https://github.com/Goreli/DKCPPDEV/blob/master/libs/dkutf/readme.md" << '\n';

	cout << "Command line options:" << '\n';
	cout << " -ef filter - exclusion filter (e.g. 1,3,5-8,11.9,12.5-7,15.3-17.21);" << '\n';
	cout << " -h         - print help info. Same as --help. Overrides all other options;" << '\n';
	cout << " -if filter - inclusion filter;" << '\n';
	cout << " -l  t      - list (g)roups, (t)ests or (b)oth along with integrity information;" << '\n';
	cout << " -r         - run unit tests contained in this executable file;" << '\n';
	cout << " -t  level  - print timing information:" << '\n';
	cout << "               0 - none (default);" << '\n';
	cout << "               1 - global level only;" << '\n';
	cout << "               2 - global level and group level;" << '\n';
	cout << "               3 - global level, group level and unit test level." << '\n';
}
