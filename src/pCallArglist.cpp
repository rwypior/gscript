#include "pCallArglist.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pListSeparator.hpp"
#include "pVar.hpp"
#include "pLiteral.hpp"
#include "pFuncCall.hpp"
#include "pStatement.hpp"

#include <iostream>
#include <vector>

namespace gscript
{
	ParserCallArglist::ParserCallArglist(const int maxCount, const int minCount, const char separator, char start, char end, BITFLAG_T allowedEntities)
		:maxCount(maxCount),
		minCount(minCount),
		separator(separator),
		start(start),
		end(end),
		allowedEntities(allowedEntities)
	{

	}

	ParseResult ParserCallArglist::parse(StringIteratorRange itrange)
	{
		ParseResult start = (ParserArglistStart(this->start)).parse(itrange);
		if (!start.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		ParserArglistEnd arglistEnd = ParserArglistEnd(this->end);
		ParseResult emptyend = arglistEnd.parse(StringIteratorRange(start.result.end, itrange.end));
		if (emptyend.isOk())
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(start.result.begin, emptyend.result.end));

		std::string::iterator begin = start.result.end;
		bool ok = true;
		int i = 0;
		do
		{
			ParserStatement stmt(true);
			ParseResult pvar = stmt.parse(StringIteratorRange(begin, itrange.end));
			if (pvar.isOk())
			{
				this->parameters.push_back(new ParserStatement(stmt));
				begin = pvar.result.end;
			}
			else
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			ParseResult pseparator = (ParserListSeparator(this->separator)).parse(StringIteratorRange(begin, itrange.end));

			if (pseparator.isOk())
				begin = pseparator.result.end;
			else
				ok = false;

			if (this->maxCount > 0 && ++i >= this->maxCount)
				break;
		} while (ok);

		ParseResult end = (ParserArglistEnd(this->end)).parse(StringIteratorRange(begin, itrange.end));

		if (!end.isOk() || i < this->minCount)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(start.result.begin, end.result.end));
	}
}