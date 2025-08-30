#include "parser/pArrayAccessor.hpp"
#include "parser/pComment.hpp"
#include "parser/pLiteral.hpp"
#include "parser/pChar.hpp"
#include "IteratorRange.hpp"

#include <cassert>

namespace gscript
{
	const char ParserArrayAccessor::KW_ARRAY_ACCESSOR_BEGIN = '[';
	const char ParserArrayAccessor::KW_ARRAY_ACCESSOR_END = ']';

	ParserArrayAccessor::ParserArrayAccessor(IndexType indexType)
		: indexType(indexType)
		, statement(true, false, false)
	{
	}

	ParseResult ParserArrayAccessor::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength));

		ParseResult beginResult = ParserChar::parse(StringIteratorRange(itrange.begin, itrange.end), ParserArrayAccessor::KW_ARRAY_ACCESSOR_BEGIN);

		if (!beginResult.isOk())
			return ParseResult(ParseResult::Status::Invalid, StringIteratorRange(beginResult.result.begin, beginResult.result.end));

		auto end = beginResult.result.end;

		if (this->indexType & IndexType::Required || this->indexType & IndexType::Optional)
		{
			ParseResult statementResult;

			if (this->indexType & IndexType::Statement)
			{
				this->statement.setAllowEmpty(!(this->indexType & IndexType::Required));
				statementResult = this->statement.parse(StringIteratorRange(beginResult.result.end, itrange.end));
			}
			else if (this->indexType & IndexType::Literal)
			{
				ParserLiteral lit;
				statementResult = lit.parse(StringIteratorRange(beginResult.result.end, itrange.end));

				if (!lit.value.empty())
					this->staticIndex = std::stoi(lit.value);
			}
			else
				assert(!"Array specifier' type must include either IndexType::Statement or IndexType::Literal");

			if (statementResult.isOk())
			{
				end = statementResult.result.end;
				this->gotValue = true;
			}
			else if (this->indexType & IndexType::Required)
				return ParseResult(ParseResult::Status::Fatal, StringIteratorRange(beginResult.result.begin, end));
		}

		ParseResult endResult = ParserChar::parse(StringIteratorRange(end, itrange.end), ParserArrayAccessor::KW_ARRAY_ACCESSOR_END);

		if (!endResult.isOk())
			return ParseResult(ParseResult::Status::Fatal, StringIteratorRange(endResult.result.begin, endResult.result.end));

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(beginResult.result.begin, endResult.result.end));
	}
}