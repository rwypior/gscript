#include "gscript/parser/pBlockStart.hpp"
#include "gscript/parser/pChar.hpp"

namespace gscript
{
	ParseResult ParserBlockStart::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, this->blockStart);
	}
}