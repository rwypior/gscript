#include "parser/pExternSpecial.hpp"

#include <string>
#include <sstream>

namespace gscript
{
	ParserExternSpecial::ParserExternSpecial(char terminator)
		: ParserSpecialDirective(ParserExternSpecial::keywordExtern, true)
		, terminator(terminator)
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
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(controlResult.result.begin, ++it));

		return ParseResult(ParseResult::Status::Invalid, { itrange, "Expected symbol name followed by semicolon" });
	}
}