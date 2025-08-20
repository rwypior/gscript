#include "parser/pMethod.hpp"
#include "parser/pAccessSpecifier.hpp"

namespace gscript
{
	ParseResult ParserMethod::parse(StringIteratorRange itrange)
	{
		auto end = itrange.begin;

		ParseResult accessres = this->accessSpecifier.parse(StringIteratorRange(end, itrange.end));
		if (accessres.isOk())
		{
			end = accessres.result.end;
		}

		ParseResult func = ParserFunction::parse(StringIteratorRange(end, itrange.end));
		return ParseResult(func.status, StringIteratorRange(itrange.begin, func.result.end));
	}
}