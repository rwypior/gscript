#include "parser/pExternSpecial.hpp"
#include "utilParserChar.hpp"
#include "utilParserWord.hpp"

#include <string>

namespace gscript
{
	const char *ParserExternSpecial::DIRECTIVE_EXTERN = "extern";

	ParserExternSpecial::ParserExternSpecial(char terminator)
		:ParserSpecialDirective(ParserExternSpecial::DIRECTIVE_EXTERN, true),
		terminator(terminator)
	{
	}

	ParseResult ParserExternSpecial::parse(StringIteratorRange itrange)
	{
		ParseResult controlResult = ParserSpecialDirective::parse(itrange);

		if (!controlResult.isOk())
			return controlResult;

		auto it = controlResult.result.end;

		bool endGood = false;
		for (; it != itrange.end; ++it)
		{
			if (*it == this->terminator)
			{
				endGood = true;
				break;
			}

			this->name += *it;
		}

		if (endGood)
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(controlResult.result.begin, ++it));

		return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
	}
}