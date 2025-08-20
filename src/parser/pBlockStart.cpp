#include "parser/pBlockStart.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	ParseResult ParserBlockStart::parse(StringIteratorRange itrange)
	{
		return Util::Char::parse(itrange, this->KW_BLOCKSTART);
	}
}