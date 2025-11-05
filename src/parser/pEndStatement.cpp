#include "gscript/parser/pEndStatement.hpp"
#include "gscript/parser/pChar.hpp"

namespace gscript
{
	ParseResult ParserEndStatement::parse(StringIteratorRange itrange)
	{
		return ParserChar::parse(itrange, this->keycharEndStatement);
	}
}