#include "pListSeparator.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	ParserListSeparator::ParserListSeparator(const char separator)
		:KW_SEPARATOR(separator)
	{

	}

	ParseResult ParserListSeparator::parse(StringIteratorRange itrange)
	{
		return Util::Char::parse(itrange, this->KW_SEPARATOR);
	}
}