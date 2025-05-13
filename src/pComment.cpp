#include "pComment.hpp"
#include "pLineComment.hpp"
#include "pBlockComment.hpp"

namespace gscript
{
	ParseResult ParserComment::parse(StringIteratorRange itrange)
	{
		ParseResult lineCommentResult = (ParserLineComment()).parse(itrange);
		if (lineCommentResult.isOk())
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(lineCommentResult.result.begin, lineCommentResult.result.end));

		ParseResult blockCommentResult = (ParserBlockComment()).parse(itrange);
		if (blockCommentResult.isOk())
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(blockCommentResult.result.begin, blockCommentResult.result.end));

		return ParseResult(ParseResult::STATUS_T::S_FATAL, itrange);
	}
}