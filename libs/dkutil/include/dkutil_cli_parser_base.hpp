/* cli_parser_base.hpp
Declares a base class for command line parsers.

Copyright(c) 2019 David Krikheli

Modification history:
    22/Nov/2019 - David Krikheli created the module.
*/

#ifndef cli_parser_base_hpp
#define cli_parser_base_hpp

#include <string>
#include <stdexcept>
#include <iostream>

namespace dk {
	class CLIParserException : public std::invalid_argument {
	public:
		CLIParserException(const std::string&);
	};

	class CLIParserBase {
	public:
		CLIParserBase(int, char**);
		virtual ~CLIParserBase();

	protected:
		void _advanceAndCheckMissingValue();
		bool _strOption(const std::string&, std::string&);
		bool _uintOption(const std::string&, size_t&);
		bool _doubleOption(const std::string&, double&);
		bool _boolOption(const std::string&, bool&) noexcept;
		bool _threeStateOption(const std::string&, bool& bFirst, const std::string&, const std::string&, bool& bSecond);

		static size_t _stringto_size_t(const std::string&) noexcept;
		static double _stringto_double(const std::string&) noexcept;

		int _argc;
		char** _argv;
		int _inxArg;
	};	// class CLIParserBase
};	// namespace dk
#endif //	cli_parser_base_hpp
