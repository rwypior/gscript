#include "parser/pVar.hpp"
#include "parser/pComment.hpp"
#include "parser/pArrayAccessor.hpp"

namespace gscript
{
	ParserVar::ParserVar(const ParserVar &copy)
		: ParserNameSpecifier(copy.name)
		, arrayAccessor(copy.arrayAccessor ? std::make_unique<ParserArrayAccessor>(*copy.arrayAccessor) : nullptr)
	{
	}

	ParseResult ParserVar::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult nameResult = ParserNameSpecifier::parse(itrange);
		if (!nameResult.isOk())
			return nameResult;

		auto end = nameResult.result.end;

		ParserArrayAccessor arrayAccessor(ParserArrayAccessor::IndexType::Required | ParserArrayAccessor::IndexType::Statement);
		ParseResult arrayResult = arrayAccessor.parse(StringIteratorRange(end, itrange.end));

		if (arrayResult.isOk())
		{
			end = arrayResult.result.end;

			this->arrayAccessor = std::make_unique<ParserArrayAccessor>(arrayAccessor);
		}
		else if (arrayResult.isFatal())
			return arrayResult;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(nameResult.result.begin, end));
	}
}