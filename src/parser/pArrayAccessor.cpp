#include "parser/pArrayAccessor.hpp"
#include "parser/pComment.hpp"
#include "parser/pLiteral.hpp"
#include "utilParserChar.hpp"
#include "IteratorRange.hpp"

#include <cassert>

namespace gscript
{
	const char ParserArrayAccessor::KW_ARRAY_ACCESSOR_BEGIN = '[';
	const char ParserArrayAccessor::KW_ARRAY_ACCESSOR_END = ']';

	ParserArrayAccessor::ParserArrayAccessor(int indexType)
		:indexType(indexType),
		statement(true, false)
	{
	}

	ParseResult ParserArrayAccessor::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

		ParseResult beginResult = Util::Char::parse(StringIteratorRange(itrange.begin, itrange.end), ParserArrayAccessor::KW_ARRAY_ACCESSOR_BEGIN);

		if (!beginResult.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange(beginResult.result.begin, beginResult.result.end));

		auto end = beginResult.result.end;

		if (this->indexType & ParserArrayAccessor::INDEX_TYPE_T::IT_REQUIRED || this->indexType & ParserArrayAccessor::INDEX_TYPE_T::IT_OPTIONAL)
		{
			ParseResult statementResult;

			if (this->indexType & ParserArrayAccessor::INDEX_TYPE_T::IT_STATEMENT)
			{
				statementResult = this->statement.parse(StringIteratorRange(beginResult.result.end, itrange.end));
			}
			else if (this->indexType & ParserArrayAccessor::IT_LITERAL)
			{
				ParserLiteral lit;
				statementResult = lit.parse(StringIteratorRange(beginResult.result.end, itrange.end));

				if (!lit.value.empty())
					this->staticIndex = std::stoi(lit.value);
			}
			else
				assert(!"Array specifier must contain either IT_REQUIRED or IT_STATEMENT bitflags");

			if (statementResult.isOk())
			{
				end = statementResult.result.end;
				this->gotValue = true;
			}
			else if (this->indexType & ParserArrayAccessor::INDEX_TYPE_T::IT_REQUIRED)
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange(beginResult.result.begin, end));
		}

		ParseResult endResult = Util::Char::parse(StringIteratorRange(end, itrange.end), ParserArrayAccessor::KW_ARRAY_ACCESSOR_END);

		if (!endResult.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange(endResult.result.begin, endResult.result.end));

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(beginResult.result.begin, endResult.result.end));
	}
}