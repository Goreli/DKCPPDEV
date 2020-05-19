/* dkutf_cli_parser.hpp
Declares the command line parser of the Unit Testing Framework library.

Copyright(c) 2020 David Krikheli

Modification history:
    13/May/2020 - David Krikheli created the module.
*/

#ifndef dkutf_cli_parser_hpp
#define dkutf_cli_parser_hpp

#include "dkutil_cli_parser_base.hpp"

namespace dk {

	class UTFCLIParser : public dk::CLIParserBase {
	public:
		UTFCLIParser(int, char**);
		void parse();
		static void printUsage() noexcept;

	private:
	};

}	// namespace dk {
#endif // dkutf_cli_parser_hpp
