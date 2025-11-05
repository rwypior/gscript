#include "gscript/parser/pBlockComment.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	ParseResult ParserBlockComment::parse(StringIteratorRange itrange)
	{
		ParseResult res = ParserWord::parse(itrange, ParserBlockComment::keycharBlockCommentBegin);

		if (res.isOk())
		{
			ParseResult resEnd = ParserWord::parseUntil(StringIteratorRange(res.result.end, itrange.end), ParserBlockComment::keycharBlockCommentEnd);

			if (resEnd.isOk())
				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(res.result.begin, resEnd.result.end + std::strlen(ParserBlockComment::keycharBlockCommentEnd)));
		}

		return ParseResult(ParseResult::Status::Invalid);
	}
}