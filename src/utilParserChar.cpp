#include "utilParserChar.hpp"

namespace gscript
{
	bool Util::Char::inString(char ch, const std::string &str)
	{
		return str.find(ch) != std::string::npos;
	}

	ParseResult Util::Char::parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		auto it = itrange.begin;
		if (ltrimWhitespaces)
		{
			while (std::isspace(*it))
			{
				++it;
			}
		}

		if (*it == c)
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(it, it + 1));

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}
}