#include "parser/pBlockStart.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	ParseResult ParserBlockStart::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, this->blockStart);
	}
}