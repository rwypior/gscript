#include "gscript/parser/pTypeSpecifier.hpp"
#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/parser/pReference.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserTypeSpecifier::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
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