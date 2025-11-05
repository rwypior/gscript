#include "gscript/parser/pIf.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParserIf::ParserIf()
		: arglist(1, 1)
	{
	}

	ParseResult ParserIf::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult parentResult = ParserWord::parse(itrange, ParserIf::keywordIf);
		if (parentResult.status != ParseResult::Status::Ok)
			return parentResult;

		parentResult.result.end = parseComment(parentResult.result.end, itrange.end);
		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return arglistres;

		auto begin = arglistres.result.end;

		begin = parseComment(begin, itrange.end);
		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return bodyres;

		begin = parseComment(begin, itrange.end);
		ParseResult elseres = this->pelse.parse(StringIteratorRange(begin, itrange.end));
		if (elseres.isOk())
			begin = elseres.result.end;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}