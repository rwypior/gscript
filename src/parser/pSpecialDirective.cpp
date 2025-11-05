#include "gscript/parser/pSpecialDirective.hpp"
#include "gscript/parser/pChar.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParserSpecialDirective::ParserSpecialDirective(const std::string &directive, bool parameterized)
		: directive(directive)
		, parameterized(parameterized)
	{
	}

	ParseResult ParserSpecialDirective::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult parentResult = ParserChar::parse(itrange, ParserSpecialDirective::keycharControl);
		if (!parentResult.isOk())
			return parentResult;

		ParseResult directiveResult = ParserWord::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			if (this->parameterized)
			{
				ParseResult paramResult = ParserChar::parse(StringIteratorRange(directiveResult.result.end, itrange.end), ParserSpecialDirective::keycharControlParam);

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