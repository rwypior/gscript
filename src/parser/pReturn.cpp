#include "parser/pReturn.hpp"
#include "parser/pStatement.hpp"
#include "utilParserWord.hpp"

namespace gscript
{
	ParseResult ParserReturn::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Word::parse(itrange, ParserReturn::KW_RETURN);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult stmt = value.parse(StringIteratorRange(parentResult.result.end + 1, itrange.end));
		if (!stmt.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, stmt.result.end));
	}
}