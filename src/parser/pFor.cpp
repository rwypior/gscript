#include "parser/pFor.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserFor::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserFor::keywordFor);
		if (parentResult.status != ParseResult::Status::Ok)
			return parentResult;

		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return arglistres;

		auto begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return bodyres;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}