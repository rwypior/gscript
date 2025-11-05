#include "gscript/parser/pArglist.hpp"
#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/parser/pComment.hpp"

#include <vector>

namespace gscript
{
	ParserArglist::ParserArglist(char start, char end)
		:arglistStart(start),
		arglistEnd(end)
	{
	}

	ParseResult ParserArglist::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult start = (ParserArglistStart(this->arglistStart)).parse(itrange);
		if (!start.isOk())
			return start;

		auto begin = start.result.end;
		bool ok = true;
		do
		{
			ParserVarDeclaration param(true);
			begin = parseComment(begin, itrange.end);
			ParseResult pvar = param.parse(StringIteratorRange(begin, itrange.end));
			if (pvar.isOk())
			{
				this->parameters.push_back(param);

				begin = pvar.result.end;
				ParseResult pseparator = (ParserListSeparator()).parse(StringIteratorRange(begin, itrange.end));

				if (pseparator.isOk())
					begin = pseparator.result.end;
				else
					ok = false;
			}
			else
				ok = false;
		} while (ok);

		begin = parseComment(begin, itrange.end);
		ParseResult end = (ParserArglistEnd(this->arglistEnd)).parse(StringIteratorRange(begin, itrange.end));

		if (!end.isOk())
			return end;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(start.result.begin, end.result.end));
	}
}