#include "parser/pInherit.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pBlockStart.hpp"

#include <sstream>

namespace gscript
{
	ParseResult ParserInherit::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::KW_INHERIT << "\", got empty string").str() });

		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it)
		{
			char chr = *it;
			if (chr == ParserInherit::KW_INHERIT)
			{
				ParseResult nameResult = (ParserNameSpecifier()).parse(StringIteratorRange(it + 1, itrange.end, itrange.getFile(), itrange.getLine()));

				return nameResult;
			}
			else if (!std::isspace(chr))
				return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::KW_INHERIT << "\", got \"" << chr << "\"").str()});
		}

		return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::KW_INHERIT << "\"").str() });
	}
}