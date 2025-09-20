#include "parser/pLineComment.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserLineComment::parse(StringIteratorRange itrange)
	{
		ParseResult res = ParserWord::parseUntil(itrange, ParserLineComment::keywordLineComment, nullptr, " \t\n");

		if (res.isOk())
		{
			ParseResult nlRes = ParserWord::parseUntil(StringIteratorRange(res.result.end, itrange.end), "\n");

			auto end = itrange.end;
			if (nlRes.isOk())
				end = nlRes.result.end;

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(res.result.begin, end));
		}

		return ParseResult(ParseResult::Status::Invalid);
	}
}