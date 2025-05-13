#include <iostream>

#include "utilParserChar.hpp"
#include "pBlockStart.hpp"

namespace gscript
{
	ParseResult ParserBlockStart::parse(StringIteratorRange itrange)
	{
		return Util::Char::parse(itrange, this->KW_BLOCKSTART);
	}
}