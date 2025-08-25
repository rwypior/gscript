#include "parser/pChar.hpp"

namespace gscript
{
	bool ParserChar::inString(char ch, const std::string &str)
	{
		return str.find(ch) != std::string::npos;
	}

	ParseResult ParserChar::parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, {itrange, "Expected \"" + std::to_string(c) + "\", got empty string"});

		auto it = itrange.begin;
		if (ltrimWhitespaces)
		{
			while (it != itrange.end && std::isspace(*it))
			{
				++it;
			}
		}

		if (it == itrange.end)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange, "Expected \"" + std::to_string(c) + "\", got empty string" });

		if (*it == c)
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(it, it + 1));

		return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange, "Expected \"" + std::to_string(c) + "\", got \"" + std::to_string(*it) + "\"" });
	}
}