#include <iostream>

#include "pFuncCall.hpp"
#include "pNameSpecifier.hpp"
#include "utilParserChar.hpp"

namespace gscript
{
	ParseResult ParserFuncCall::parse(StringIteratorRange itrange)
	{
		std::string::iterator nameEnd;
		std::string::iterator begin = itrange.end;

		ParserNameSpecifier nameSpecifier;
		ParseResult name = nameSpecifier.parse(itrange);
		if (!name.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		begin = name.result.begin;
		nameEnd = name.result.end;

		//this->name = name.getWord();
		this->name = nameSpecifier.name;

		ParseResult arglist = this->arglist.parse(StringIteratorRange(nameEnd, itrange.end));
		if (!arglist.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(begin, arglist.result.end));
	}
}