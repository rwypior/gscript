#ifndef _h_gscript_parser_char
#define _h_gscript_parser_char

#include "parser/pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	namespace ParserChar
	{
		bool inString(char ch, const std::string &str);
		ParseResult parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces = true);
	};
}

#endif