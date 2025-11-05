#include "gscript/parser/pArrayInitializer.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/parser/pLiteral.hpp"

namespace gscript
{
	ParserArrayInitializer::ParserArrayInitializer()
		: arglist(0, 0, ',', keycharArrayInitializerBegin, keycharArrayInitializerEnd)
	{
	}

	ParseResult ParserArrayInitializer::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, { itrange, "Expected array initializer, got empty string" });

		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult arglistResult = this->arglist.parse(itrange);

		return arglistResult;
	}
}