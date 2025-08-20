#include "parser/pVar.hpp"
#include "parser/pComment.hpp"
#include "parser/pArrayAccessor.hpp"

namespace gscript
{
	ParserVar::ParserVar(const ParserVar &copy)
		:ParserNameSpecifier(copy.name),
		arrayAccessor(copy.arrayAccessor ? new ParserArrayAccessor(*copy.arrayAccessor) : NULL)
	{
	}

	ParserVar::~ParserVar()
	{
		if (this->arrayAccessor)
			delete this->arrayAccessor;
	}

	ParseResult ParserVar::parse(StringIteratorRange itrange)
	{
		ParseResult nameResult = ParserNameSpecifier::parse(itrange);
		if (!nameResult.isOk())
			return nameResult;

		auto end = nameResult.result.end;

		ParserArrayAccessor arrayAccessor(ParserArrayAccessor::INDEX_TYPE_T::IT_REQUIRED | ParserArrayAccessor::INDEX_TYPE_T::IT_STATEMENT);
		ParseResult arrayResult = arrayAccessor.parse(StringIteratorRange(end, itrange.end));

		if (arrayResult.isOk())
		{
			end = arrayResult.result.end;

			this->arrayAccessor = new ParserArrayAccessor(arrayAccessor);
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(nameResult.result.begin, end));
	}
}