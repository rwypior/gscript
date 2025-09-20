#include "parser/pArrayInitializer.hpp"
#include "parser/pComment.hpp"
#include "parser/pLiteral.hpp"

namespace gscript
{
	ParserArrayInitializer::ParserArrayInitializer()
		: arglist(0, 0, ',', keycharArrayInitializerBegin, keycharArrayInitializerEnd)
	{
	}

	ParseResult ParserArrayInitializer::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, COMMENT_RESULT(itrange, commentLength));

		ParseResult arglistResult = this->arglist.parse(itrange);

		return arglistResult;
	}
}