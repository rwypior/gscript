#include "parser/pControlDirective.hpp"
#include "parser/pChar.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParserControlDirective::ParserControlDirective(const std::string &directive)
		: directive(directive)
	{
	}

	ParseResult ParserControlDirective::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserChar::parse(itrange, ParserControlDirective::keycharControl);
		if (parentResult.status != ParseResult::Status::Ok)
			return parentResult;

		ParseResult directiveResult = ParserWord::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, directiveResult.result.end));
		}

		return directiveResult;
	}
}