#include "parser/pReference.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	const char ParserReference::KW_REFERENCE = '&';

	ParseResult ParserReference::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, ParserReference::KW_REFERENCE);
	}
}