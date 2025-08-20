#include "parser/pBlockComment.hpp"
#include "utilParserWord.hpp"
#include "utilParserChar.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	ParseResult ParserBlockComment::parse(StringIteratorRange itrange)
	{
		ParseResult res = Util::Word::parse(itrange, ParserBlockComment::CHR_BLOCK_COMMENT_BEGIN);

		if (res.isOk())
		{
			ParseResult resEnd = Util::Word::parseUntil(StringIteratorRange(res.result.end, itrange.end), ParserBlockComment::CHR_BLOCK_COMMENT_END);

			if (resEnd.isOk())
				return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(res.result.begin, resEnd.result.end));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}
}