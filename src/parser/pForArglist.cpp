#include "gscript/parser/pForArglist.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pStatement.hpp"

#include <vector>

namespace gscript
{
	ParserForArglist::ParserForArglist(const char separator, char start, char end)
		: separator(separator)
		, start(start)
		, end(end)
	{
	}

	ParseResult ParserForArglist::parse(StringIteratorRange itrange)
	{
		ParseResult start = (ParserArglistStart(this->start)).parse(itrange);
		if (!start.isOk())
			return start;

		auto begin = start.result.end;

		ParserVarDeclaration vardecl(true);
		ParseResult pvarres = vardecl.parse(StringIteratorRange(begin, itrange.end));
		if (pvarres.isOk())
		{
			this->varDecl = new ParserVarDeclaration(vardecl);
			begin = pvarres.result.end;
		}

		ParseResult pseparator = (ParserListSeparator(this->separator)).parse(StringIteratorRange(begin, itrange.end));
		if (!pseparator.isOk())
			return pseparator;

		ParserStatement cond(true);
		ParseResult pcondres = cond.parse(StringIteratorRange(pseparator.result.end, itrange.end));
		if (pcondres.isOk())
		{
			this->condition = new ParserStatement(cond);
			begin = pcondres.result.end;
		}

		ParseResult pseparator2 = (ParserListSeparator(this->separator)).parse(StringIteratorRange(begin, itrange.end));
		if (!pseparator2.isOk())
			return pseparator2;

		ParserStatement progress(true);
		ParseResult pprogressres = progress.parse(StringIteratorRange(pseparator2.result.end, itrange.end));
		if (pprogressres.isOk())
		{
			this->progress = new ParserStatement(progress);
			begin = pprogressres.result.end;
		}

		ParseResult end = (ParserArglistEnd(this->end)).parse(StringIteratorRange(begin, itrange.end));

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(start.result.begin, end.result.end));
	}
}