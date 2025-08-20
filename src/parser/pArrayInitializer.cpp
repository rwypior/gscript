#include "parser/pArrayInitializer.hpp"
#include "parser/pComment.hpp"
#include "parser/pLiteral.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	const char ParserArrayInitializer::KW_ARRAY_INITIALIZER_BEGIN = '{';
	const char ParserArrayInitializer::KW_ARRAY_INITIALIZER_END = '}';

	ParserArrayInitializer::ParserArrayInitializer()
		:arglist(0, 0, ',', KW_ARRAY_INITIALIZER_BEGIN, KW_ARRAY_INITIALIZER_END)
	{ }

	ParseResult ParserArrayInitializer::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

		ParseResult arglistResult = this->arglist.parse(itrange);

		return arglistResult;
	}
}