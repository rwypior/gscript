#include "parser/pEndStatement.hpp"
#include "parser/pChar.hpp"

namespace gscript
{
	ParseResult ParserEndStatement::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, this->KW_ENDSTATEMENT);
	}
}