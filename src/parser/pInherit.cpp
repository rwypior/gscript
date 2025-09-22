#include "parser/pInherit.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pBlockStart.hpp"
#include "parser/pComment.hpp"

#include <sstream>

namespace gscript
{
	ParseResult ParserInherit::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::keycharInherit << "\", got empty string").str() });

		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it)
		{
			it = parseComment(it, itrange.end);
			char chr = *it;
			if (chr == ParserInherit::keycharInherit)
			{
				it = parseComment(it + 1, itrange.end);
				ParseResult nameResult = (ParserNameSpecifier()).parse(StringIteratorRange(it, itrange.end, itrange.getFile(), itrange.getLine()));

				return nameResult;
			}
			else if (!std::isspace(chr))
				return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::keycharInherit << "\", got \"" << chr << "\"").str()});
		}

		return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected \"" << ParserInherit::keycharInherit << "\"").str() });
	}
}