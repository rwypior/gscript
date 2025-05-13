#include <iostream>

#include "pMethod.hpp"
#include "pAccessSpecifier.hpp"

namespace gscript
{
	ParseResult ParserMethod::parse(StringIteratorRange itrange)
	{
		std::string::iterator end = itrange.begin;

		ParseResult accessres = this->accessSpecifier.parse(StringIteratorRange(end, itrange.end));
		if (accessres.isOk())
		{
			end = accessres.result.end;
		}

		ParseResult func = ParserFunction::parse(StringIteratorRange(end, itrange.end));
		return ParseResult(func.status, ParserEntity::StringIteratorRange(itrange.begin, func.result.end));
	}
}