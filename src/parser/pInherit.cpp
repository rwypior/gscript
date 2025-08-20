#include "parser/pInherit.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pBlockStart.hpp"

namespace gscript
{
	ParseResult ParserInherit::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it)
		{
			char chr = *it;
			if (chr == ParserInherit::KW_INHERIT)
			{
				ParseResult nameResult = (ParserNameSpecifier()).parse(StringIteratorRange(it + 1, itrange.end));

				return nameResult;
			}
			else if (chr != ' ' || chr != '\n' || chr != '\t')
				return ParseResult(ParseResult::STATUS_T::S_FATAL);
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}
}