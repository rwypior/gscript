#include "parser/pAbstractSpecial.hpp"

#include <string>
#include <cctype>

namespace gscript
{
	const char *ParserAbstractSpecial::DIRECTIVE_ABSTRACT = "abstract";

	ParserAbstractSpecial::ParserAbstractSpecial()
		: ParserSpecialDirective(ParserAbstractSpecial::DIRECTIVE_ABSTRACT, false)
	{
	}

	ParseResult ParserAbstractSpecial::parse(StringIteratorRange itrange)
	{
		return ParserSpecialDirective::parse(itrange);
	}
}