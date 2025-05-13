#include <iostream>

#include "pIf.hpp"
#include "utilParserWord.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pCallArglist.hpp"

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
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		std::string::iterator begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		ParseResult elseres = this->pelse.parse(StringIteratorRange(begin, itrange.end));
		if (elseres.isOk())
			begin = elseres.result.end;

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(parentResult.result.begin, begin));
	}
}