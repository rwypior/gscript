#include "parser/pChar.hpp"
#include "StringUtils.hpp"

#include <sstream>

namespace gscript
{
	bool ParserChar::inString(char ch, const std::string &str)
	{
		return str.find(ch) != std::string::npos;
	}

	ParseResult ParserChar::parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, {itrange, (std::stringstream() << "Expected \"" << c << "\", got empty string").str() });

		size_t newlines = 0;
		auto it = itrange.begin;
		if (ltrimWhitespaces)
		{
			while (it != itrange.end && std::isspace(*it))
			{
				newlines += isNewLine(*it);
				++it;
			}
		}

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Invalid, {itrange.shifted(newlines), (std::stringstream() << "Expected \"" << c << "\", got empty string").str()});

		if (*it == c)
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(it, it + 1, itrange.getFile(), itrange.getLine()), nullptr, { newlines });

		return ParseResult(ParseResult::Status::Invalid, {itrange.shifted(newlines), (std::stringstream() << "Expected \"" << c << "\", got \"" << *it << "\"").str()});
	}
}