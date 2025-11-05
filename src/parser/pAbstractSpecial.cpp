#include "gscript/parser/pAbstractSpecial.hpp"

#include <string>
#include <cctype>

namespace gscript
{
	ParserAbstractSpecial::ParserAbstractSpecial()
		: ParserSpecialDirective(ParserAbstractSpecial::keywordAbstract, false)
	{
	}

	ParseResult ParserAbstractSpecial::parse(StringIteratorRange itrange)
	{
		return ParserSpecialDirective::parse(itrange);
	}
}