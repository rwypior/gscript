#ifndef _h_util_p_word
#define _h_util_p_word

#include <string>

#include "pEntity.hpp"

namespace gscript
{
	class ParseResult;

	namespace Util
	{
		namespace Word
		{
			extern const std::string WORD_ANY;

			ParseResult parse(ParserEntity::StringIteratorRange itrange, const std::string &word, void *subResult = NULL, bool allowSpaces = false);
			ParseResult parseUntil(ParserEntity::StringIteratorRange itrange, const std::string &word, void *subResult = NULL, const std::string &allowed = "");
			void copy(char *destination, ParserEntity::StringIteratorRange itrange);
		};
	}
}

#endif