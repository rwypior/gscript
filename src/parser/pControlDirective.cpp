#include "parser/pControlDirective.hpp"
#include "utilParserChar.hpp"
#include "utilParserWord.hpp"

namespace gscript
{
	const char ParserControlDirective::C_CONTROL = '#';

	ParserControlDirective::ParserControlDirective(const std::string &directive)
		:directive(directive)
	{
	}

	ParseResult ParserControlDirective::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Char::parse(itrange, ParserControlDirective::C_CONTROL);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult directiveResult = Util::Word::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, directiveResult.result.end));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
	}
}