#ifndef _h_gscript_util_p_char
#define _h_gscript_util_p_char

#include "parser/pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	namespace Util
	{
		namespace Char
		{
			bool inString(char ch, const std::string &str);
			ParseResult parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces = true);
		};
	}
}

#endif