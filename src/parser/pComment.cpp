#include "parser/pComment.hpp"
#include "parser/pLineComment.hpp"
#include "parser/pBlockComment.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	ParserComment::ParserComment(bool mergeLineComments)
		: mergeLineComments(mergeLineComments)
	{
	}

	ParseResult ParserComment::parse(StringIteratorRange itrange)
	{
		ParseResult lineCommentResult = (ParserLineComment()).parse(itrange);
		if (lineCommentResult.isOk())
		{
			if (this->mergeLineComments)
			{
				auto it = lineCommentResult.result.end;
				while (it != itrange.end)
				{
					auto nonWhitespaceResult = ParserChar::parseUntilNonWhitespace(StringIteratorRange(it, itrange.end));

					if (!nonWhitespaceResult.isOk())
						break;

					it = nonWhitespaceResult.result.begin;
					auto result = (ParserLineComment()).parse(StringIteratorRange(it, itrange.end));

					if (!result.isOk())
						break;

					it = result.result.end;
				}
				lineCommentResult.result.end = it;
			}
			
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(lineCommentResult.result.begin, lineCommentResult.result.end));
		}

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