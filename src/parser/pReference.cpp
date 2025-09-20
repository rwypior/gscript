#include "parser/pReference.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	ParseResult ParserReference::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, ParserReference::keycharReference);
	}
}