#include <iostream>

#include "pWhile.hpp"
#include "utilParserWord.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pCallArglist.hpp"

namespace gscript
{
	const char *ParserWhile::KW_WHILE = "while";

	ParserWhile::ParserWhile()
		:arglist(1, 1)
	{
	}

	ParseResult ParserWhile::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Word::parse(itrange, ParserWhile::KW_WHILE);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		std::string::iterator begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(parentResult.result.begin, begin));
	}
}