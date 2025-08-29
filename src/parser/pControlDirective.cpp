#include "parser/pControlDirective.hpp"
#include "parser/pChar.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	const char ParserControlDirective::C_CONTROL = '#';

	ParserControlDirective::ParserControlDirective(const std::string &directive)
		: directive(directive)
	{
	}

	ParseResult ParserControlDirective::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserChar::parse(itrange, ParserControlDirective::C_CONTROL);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult directiveResult = ParserWord::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, directiveResult.result.end));
		}

		return directiveResult;
	}
}