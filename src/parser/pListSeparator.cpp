#include "parser/pListSeparator.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	ParserListSeparator::ParserListSeparator(const char separator)
		: separatorChar(separator)
	{
	}

	ParseResult ParserListSeparator::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, this->separatorChar);
	}
}