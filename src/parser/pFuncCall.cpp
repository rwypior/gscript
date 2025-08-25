#include "parser/pFuncCall.hpp"
#include "parser/pNameSpecifier.hpp"

namespace gscript
{
	ParseResult ParserFuncCall::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.end;

		ParserNameSpecifier nameSpecifier;
		ParseResult name = nameSpecifier.parse(itrange);
		if (!name.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		begin = name.result.begin;
		auto nameEnd = name.result.end;

		//this->name = name.getWord();
		this->name = nameSpecifier.name;

		ParseResult arglist = this->arglist.parse(StringIteratorRange(nameEnd, itrange.end));
		if (!arglist.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(begin, arglist.result.end));
	}
}