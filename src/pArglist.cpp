#include <iostream>

#include "pEntity.hpp"
#include "pArglist.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pVarDeclaration.hpp"
#include "pListSeparator.hpp"

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
		ParseResult start = (ParserArglistStart(this->arglistStart)).parse(itrange);
		if (!start.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		std::string::iterator begin = start.result.end;
		bool ok = true;
		do
		{
			ParserVarDeclaration param(true);
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

		ParseResult end = (ParserArglistEnd(this->arglistEnd)).parse(StringIteratorRange(begin, itrange.end));

		if (!end.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(start.result.begin, end.result.end));
	}
}