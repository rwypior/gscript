#include "parser/pEndStatement.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	ParseResult ParserEndStatement::parse(StringIteratorRange itrange)
	{
		return Util::Char::parse(itrange, this->KW_ENDSTATEMENT);
	}
}