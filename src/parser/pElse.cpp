#include "parser/pElse.hpp"
#include "parser/pIf.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserElse::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserElse::KW_ELSE);
		if (parentResult.status != ParseResult::Status::Ok)
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
				return statementres;

			begin = statementres.result.end;
		}

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}