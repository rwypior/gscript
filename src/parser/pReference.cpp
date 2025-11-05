#include "gscript/parser/pReference.hpp"
#include "gscript/parser/pChar.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserReference::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		return ParserChar::parse(itrange, ParserReference::keycharReference);
	}
}