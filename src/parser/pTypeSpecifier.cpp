#include "parser/pTypeSpecifier.hpp"
#include "parser/pArrayAccessor.hpp"
#include "parser/pReference.hpp"

namespace gscript
{
	ParseResult ParserTypeSpecifier::parse(StringIteratorRange itrange)
	{
		ParseResult nameResult = ParserNameSpecifier::parse(itrange);
		auto end = nameResult.result.end;

		if (nameResult.isOk())
		{
			ParserArrayAccessor arrayAccessor = ParserArrayAccessor(ParserArrayAccessor::IndexType::Optional | ParserArrayAccessor::IndexType::Literal);
			ParseResult arrayResult = arrayAccessor.parse(StringIteratorRange(nameResult.result.end, itrange.end));

			if (arrayResult.isOk())
			{
				this->isArray = true;
				end = arrayResult.result.end;
			}

			ParserReference ref;
			ParseResult refResult = ref.parse(StringIteratorRange(end, itrange.end));

			if (refResult.isOk())
			{
				this->isReference = true;
				end = refResult.result.end;
			}

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(nameResult.result.begin, end));
		}

		return nameResult;
	}
}