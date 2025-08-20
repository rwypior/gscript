#include "parser/pLineComment.hpp"
#include "utilParserWord.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	ParseResult ParserLineComment::parse(StringIteratorRange itrange)
	{
		ParseResult res = Util::Word::parseUntil(itrange, ParserLineComment::CHR_LINE_COMMENT, nullptr, " \t\n");

		if (res.isOk())
		{
			ParseResult nlRes = Util::Word::parseUntil(StringIteratorRange(res.result.end, itrange.end), "\n");

			auto end = itrange.end;
			if (nlRes.isOk())
				end = nlRes.result.end;

			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(res.result.begin, end));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}
}