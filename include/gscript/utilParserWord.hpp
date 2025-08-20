#ifndef _h_gscript_util_p_word
#define _h_gscript_util_p_word

#include "parser/pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ParseResult;

	namespace Util
	{
		namespace Word
		{
			extern const std::string WORD_ANY;

			ParseResult parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult = nullptr, bool allowSpaces = false);
			ParseResult parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult = nullptr, const std::string &allowed = "");
			void copy(char *destination, StringIteratorRange itrange);
		};
	}
}

#endif