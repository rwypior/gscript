#include "parser/pBlockComment.hpp"
#include "parser/pWord.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	ParseResult ParserBlockComment::parse(StringIteratorRange itrange)
	{
		ParseResult res = ParserWord::parse(itrange, ParserBlockComment::CHR_BLOCK_COMMENT_BEGIN);

		if (res.isOk())
		{
			ParseResult resEnd = ParserWord::parseUntil(StringIteratorRange(res.result.end, itrange.end), ParserBlockComment::CHR_BLOCK_COMMENT_END);

			if (resEnd.isOk())
				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(res.result.begin, resEnd.result.end));
		}

		return ParseResult(ParseResult::Status::Invalid);
	}
}