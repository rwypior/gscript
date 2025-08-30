#include "parser/pIf.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParserIf::ParserIf()
		:arglist(1, 1)
	{
	}

	ParseResult ParserIf::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserIf::KW_IF);
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

		ParseResult elseres = this->pelse.parse(StringIteratorRange(begin, itrange.end));
		if (elseres.isOk())
			begin = elseres.result.end;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}