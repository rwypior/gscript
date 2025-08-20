#include "parser/pElse.hpp"
#include "parser/pIf.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "utilParserWord.hpp"

namespace gscript
{
	ParseResult ParserElse::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = Util::Word::parse(itrange, ParserElse::KW_ELSE);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		auto begin = parentResult.result.end;

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
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			begin = statementres.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, begin));
	}
}