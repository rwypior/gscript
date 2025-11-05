#include "gscript/parser/pMethod.hpp"
#include "gscript/parser/pAccessSpecifier.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserMethod::parse(StringIteratorRange itrange)
	{
		auto end = itrange.begin;

		end = parseComment(end, itrange.end);
		ParseResult accessres = this->accessSpecifier.parse(StringIteratorRange(end, itrange.end));
		if (accessres.isOk())
		{
			end = accessres.result.end;
		}

		return ParserFunction::parse(StringIteratorRange(end, itrange.end));
	}
}