#ifndef _h_gscript_parser_char
#define _h_gscript_parser_char

#include "parser/pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	/// Holds a set of utility functions for parsing single characters
	namespace ParserChar
	{
		bool inString(char ch, const std::string &str);

		/// Parse until the character 'c' is encountered and return the 'c' character position
		/// @param itrange String range to parse
		/// @param c The character to find
		/// @param ltrimWhitespaces When set to true, allow whitespaces on the left side of 'c'
		/// when set to false, this function will fail if 'c' is not the first character
		ParseResult parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces = true);

		/// Find first non-whitespace character
		/// @param itrange String range to parse
		ParseResult parseUntilNonWhitespace(StringIteratorRange itrange);
	};
}

#endif