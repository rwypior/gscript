#include "parser/pReference.hpp"
#include "parser/pChar.hpp"
#include "parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserReference::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		return ParserChar::parse(itrange, ParserReference::keycharReference);
	}
}