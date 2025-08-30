#include "parser/pSpecialDirective.hpp"
#include "parser/pChar.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	const char ParserSpecialDirective::C_CONTROL = '@';
	const char ParserSpecialDirective::C_CONTROL_PARAM = ':';
	const char ParserSpecialDirective::C_CONTROL_PARAM_TERMINATOR = '\n';

	ParserSpecialDirective::ParserSpecialDirective(const std::string &directive, bool parameterized)
		:directive(directive),
		parameterized(parameterized)
	{
	}

	ParseResult ParserSpecialDirective::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserChar::parse(itrange, ParserSpecialDirective::C_CONTROL);
		if (!parentResult.isOk())
			return parentResult;

		ParseResult directiveResult = ParserWord::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			if (this->parameterized)
			{
				ParseResult paramResult = ParserChar::parse(StringIteratorRange(directiveResult.result.end, itrange.end), ParserSpecialDirective::C_CONTROL_PARAM);

				if (paramResult.isOk())
					return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, paramResult.result.end));
				else
					return paramResult;
			}

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, directiveResult.result.end));
		}

		return directiveResult;
	}
}