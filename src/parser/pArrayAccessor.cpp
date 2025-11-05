#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pChar.hpp"
#include "gscript/IteratorRange.hpp"

#include <cassert>

namespace gscript
{
	ParserArrayAccessor::ParserArrayAccessor(IndexType indexType)
		: indexType(indexType)
		, statement(true, false, false)
	{
	}

	ParseResult ParserArrayAccessor::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, { itrange, "Expected array accessor, got empty string" });

		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult beginResult = ParserChar::parse(StringIteratorRange(itrange.begin, itrange.end), ParserArrayAccessor::keycharArrayAccessorBegin);

		if (!beginResult.isOk())
			return beginResult;

		auto end = beginResult.result.end;

		if (this->indexType & IndexType::Required || this->indexType & IndexType::Optional)
		{
			ParseResult statementResult;

			if (this->indexType & IndexType::Statement)
			{
				this->statement.setAllowEmpty(!(this->indexType & IndexType::Required));
				beginResult.result.end = parseComment(beginResult.result.end, itrange.end);
				statementResult = this->statement.parse(StringIteratorRange(beginResult.result.end, itrange.end));
			}
			else if (this->indexType & IndexType::Literal)
			{
				ParserLiteral lit;
				beginResult.result.end = parseComment(beginResult.result.end, itrange.end);
				statementResult = lit.parse(StringIteratorRange(beginResult.result.end, itrange.end));

				if (!lit.value.empty())
					this->staticIndex = std::stoi(lit.value);
			}
			else
				assert(!"Array specifier's type must include either IndexType::Statement or IndexType::Literal");

			if (statementResult.isOk())
			{
				end = statementResult.result.end;
				this->gotValue = true;
			}
			else if (this->indexType & IndexType::Required)
				return statementResult.as(ParseResult::Status::Fatal);
		}

		end = parseComment(end, itrange.end);
		ParseResult endResult = ParserChar::parse(StringIteratorRange(end, itrange.end), ParserArrayAccessor::keycharArrayAccessorEnd);

		if (!endResult.isOk())
			return endResult.as(ParseResult::Status::Fatal);

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(beginResult.result.begin, endResult.result.end));
	}
}