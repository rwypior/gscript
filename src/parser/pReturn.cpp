#include "parser/pReturn.hpp"
#include "parser/pStatement.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserReturn::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserReturn::keywordReturn);
		if (!parentResult.isOk())
			return parentResult;

		ParseResult stmt = value.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!stmt.isOk())
			return stmt;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, stmt.result.end));
	}
}