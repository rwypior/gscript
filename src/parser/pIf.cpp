#include "parser/pIf.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "utilParserWord.hpp"

namespace gscript
{
	ParserIf::ParserIf()
		:arglist(1, 1)
	{
	}

	ParseResult ParserIf::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Word::parse(itrange, ParserIf::KW_IF);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		auto begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		ParseResult elseres = this->pelse.parse(StringIteratorRange(begin, itrange.end));
		if (elseres.isOk())
			begin = elseres.result.end;

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, begin));
	}
}