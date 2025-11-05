#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserReturn::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult parentResult = ParserWord::parse(itrange, ParserReturn::keywordReturn);
		if (!parentResult.isOk())
			return parentResult;

		ParseResult stmt = value.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!stmt.isOk())
			return stmt;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, stmt.result.end));
	}
}