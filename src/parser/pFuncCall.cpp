#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserFuncCall::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.end;

		ParserNameSpecifier nameSpecifier;
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult name = nameSpecifier.parse(itrange);
		if (!name.isOk())
			return name;

		begin = name.result.begin;
		auto nameEnd = name.result.end;

		this->name = nameSpecifier.name;

		ParseResult arglist = this->arglist.parse(StringIteratorRange(nameEnd, itrange.end));
		if (!arglist.isOk())
			return arglist;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(begin, arglist.result.end));
	}
}