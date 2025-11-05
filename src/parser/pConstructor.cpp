#include "gscript/parser/pConstructor.hpp"
#include "gscript/parser/pClass.hpp"

namespace gscript
{
	ParserConstructor::ParserConstructor(ParserClass &pClass)
		: pClass(pClass)
	{
	}

	ParseResult ParserConstructor::parse(StringIteratorRange itrange)
	{
		ParseResult res = ParserMethod::parse(itrange);

		this->returnTypeName = this->pClass.name;

		return res;
	}
}