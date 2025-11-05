#include "gscript/parser/pElse.hpp"
#include "gscript/parser/pIf.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserElse::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult parentResult = ParserWord::parse(itrange, ParserElse::keywordElse);
		if (parentResult.status != ParseResult::Status::Ok)
			return parentResult;

		auto begin = parentResult.result.end;

		begin = parseComment(begin, itrange.end);
		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
		{
			begin = bodyres.result.end;
		}
		else
		{
			ParserStatement statement;
			begin = parseComment(begin, itrange.end);
			ParseResult statementres = statement.parse(StringIteratorRange(begin, itrange.end));

			if (!statementres.isOk())
				return statementres;

			begin = statementres.result.end;
		}

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, begin));
	}
}