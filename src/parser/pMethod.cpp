#include "parser/pMethod.hpp"
#include "parser/pAccessSpecifier.hpp"
#include "parser/pComment.hpp"

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