#include "pReference.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	const char ParserReference::KW_REFERENCE = '&';

	ParseResult ParserReference::parse(StringIteratorRange itrange)
	{
		return Util::Char::parse(itrange, ParserReference::KW_REFERENCE);
	}
}