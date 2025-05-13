#include <iostream>

#include "pElse.hpp"
#include "pIf.hpp"
#include "utilParserWord.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pCallArglist.hpp"

namespace gscript
{
	ParseResult ParserElse::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Word::parse(itrange, ParserElse::KW_ELSE);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		std::string::iterator begin = parentResult.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
		{
			begin = bodyres.result.end;
		}
		else
		{
			ParserStatement statement;
			ParseResult statementres = statement.parse(StringIteratorRange(begin, itrange.end));

			if (!statementres.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

			begin = statementres.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(parentResult.result.begin, begin));
	}
}