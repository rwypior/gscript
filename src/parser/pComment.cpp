#include "parser/pComment.hpp"
#include "parser/pLineComment.hpp"
#include "parser/pBlockComment.hpp"

namespace gscript
{
	ParseResult ParserComment::parse(StringIteratorRange itrange)
	{
		ParseResult lineCommentResult = (ParserLineComment()).parse(itrange);
		if (lineCommentResult.isOk())
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(lineCommentResult.result.begin, lineCommentResult.result.end));

		ParseResult blockCommentResult = (ParserBlockComment()).parse(itrange);
		if (blockCommentResult.isOk())
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(blockCommentResult.result.begin, blockCommentResult.result.end));

		return ParseResult(ParseResult::Status::Invalid, itrange);
	}

	StringIteratorRange::ITERATOR_T parseComment(StringIteratorRange itrange)
	{
		auto res = ParserComment().parse(itrange);
		if (res.isOk())
			return res.result.end;
		return itrange.begin;
	}

	StringIteratorRange::ITERATOR_T parseComment(StringIteratorRange::ITERATOR_T begin, StringIteratorRange::ITERATOR_T end)
	{
		return parseComment(StringIteratorRange(begin, end));
	}
}