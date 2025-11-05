#include "gscript/parser/pFieldDeclaration.hpp"
#include "gscript/parser/pAccessSpecifier.hpp"

namespace gscript
{
	ParseResult ParserFieldDeclaration::parse(StringIteratorRange itrange)
	{
		auto end = itrange.begin;

		ParseResult accessres = this->accessSpecifier.parse(StringIteratorRange(end, itrange.end));
		if (accessres.isOk())
		{
			end = accessres.result.end;
		}

		ParseResult func = ParserVarDeclaration::parse(StringIteratorRange(end, itrange.end));
		return func;
	}
}