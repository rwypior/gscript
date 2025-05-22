#ifndef _h_gscript_util_p_char
#define _h_gscript_util_p_char

#include <string>
#include "pEntity.hpp"

namespace gscript
{
	namespace Util
	{
		namespace Char
		{
			bool inString(char ch, const std::string &str);
			ParseResult parse(ParserEntity::StringIteratorRange itrange, char c, bool ltrimWhitespaces = true);
		};
	}
}

#endif