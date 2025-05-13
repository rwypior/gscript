#include "pSpecialDirective.hpp"

#include "utilParserChar.hpp"
#include "utilParserWord.hpp"

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
		ParseResult parentResult = Util::Char::parse(itrange, ParserSpecialDirective::C_CONTROL);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult directiveResult = Util::Word::parse(StringIteratorRange(parentResult.result.end, itrange.end), this->directive);

		if (directiveResult.isOk())
		{
			if (this->parameterized)
			{
				ParseResult paramResult = Util::Char::parse(StringIteratorRange(directiveResult.result.end, itrange.end), ParserSpecialDirective::C_CONTROL_PARAM);

				if (paramResult.isOk())
					return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(parentResult.result.begin, paramResult.result.end));
				else
					return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());
			}

			return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(parentResult.result.begin, directiveResult.result.end));
		}

		return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());
	}
}