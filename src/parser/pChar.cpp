#include "gscript/parser/pChar.hpp"
#include "gscript/StringUtils.hpp"

#include <sstream>

namespace gscript
{
	namespace ParserChar
	{
		bool inString(char ch, const std::string& str)
		{
			return str.find(ch) != std::string::npos;
		}

		ParseResult parse(StringIteratorRange itrange, char c, bool ltrimWhitespaces)
		{
			if (itrange.end - itrange.begin < 1)
				return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << c << "\", got empty string").str() });

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
				return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << c << "\", got empty string").str() });

			if (*it == c)
				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(it, it + 1, itrange.getFile(), itrange.getLine()), nullptr, { newlines });

			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << c << "\", got \"" << *it << "\"").str() });
		}

		ParseResult parseUntilNonWhitespace(StringIteratorRange itrange)
		{
			if (itrange.end - itrange.begin < 1)
				return ParseResult(ParseResult::Status::Invalid, { itrange, "Expected non-whitespace character, got empty string" });

			size_t newlines = 0;
			auto it = itrange.begin;
			while (it != itrange.end && std::isspace(*it))
			{
				newlines += isNewLine(*it);
				it++;
			}

			if (it == itrange.end)
				return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), "Expected non-whitespace character, got empty string" });

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(it, it + 1, itrange.getFile(), itrange.getLine()), nullptr, { newlines });
		}
	}
}