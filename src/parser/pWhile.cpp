#include "gscript/parser/pWhile.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParserWhile::ParserWhile()
		: arglist(1, 1)
	{
	}

	ParseResult ParserWhile::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult parentResult = ParserWord::parse(itrange, ParserWhile::keywordWhile);
		if (!parentResult.isOk())
			return parentResult;

		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return arglistres.as(ParseResult::Status::Fatal);

		auto begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return bodyres.as(ParseResult::Status::Fatal);

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}