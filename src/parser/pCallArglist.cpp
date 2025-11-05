#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/StringUtils.hpp"

#include <sstream>

namespace gscript
{
	ParserCallArglist::ParserCallArglist(const int maxCount, const int minCount, const char separator, char start, char end)
		: maxCount(maxCount)
		, minCount(minCount)
		, separator(separator)
		, start(start)
		, end(end)
	{
	}

	ParseResult ParserCallArglist::parse(StringIteratorRange itrange)
	{
		if (itrange.end - itrange.begin < 1)
			return ParseResult(ParseResult::Status::Invalid, {itrange, "Expected argument list"});
		
		if (itrange.end - itrange.begin < 2)
			return ParseResult(ParseResult::Status::Invalid, {itrange, "Argument list must contain opening and closing characters"});

		ParseResult start = (ParserArglistStart(this->start)).parse(itrange);
		if (!start.isOk())
			return ParseResult(ParseResult::Status::Invalid, StringIteratorRange());

		ParserArglistEnd arglistEnd = ParserArglistEnd(this->end);
		start.result.end = parseComment(start.result.end, itrange.end);
		ParseResult emptyend = arglistEnd.parse(StringIteratorRange(start.result.end, itrange.end));
		if (emptyend.isOk())
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(start.result.begin, emptyend.result.end));

		auto begin = start.result.end;
		bool ok = true;
		int i = 0;
		do
		{
			ParserStatement stmt(true);
			begin = parseComment(begin, itrange.end);
			ParseResult pvar = stmt.parse(StringIteratorRange(begin, itrange.end));
			if (pvar.isOk())
			{
				this->parameters.push_back(std::make_unique<ParserStatement>(stmt));
				begin = pvar.result.end;
			}
			else
				return pvar;

			begin = parseComment(begin, itrange.end);
			ParseResult pseparator = (ParserListSeparator(this->separator)).parse(StringIteratorRange(begin, itrange.end));

			if (pseparator.isOk())
				begin = pseparator.result.end;
			else
				ok = false;

			if (this->maxCount > 0 && ++i >= this->maxCount)
				break;
		} while (ok);

		begin = parseComment(begin, itrange.end);
		ParseResult end = (ParserArglistEnd(this->end)).parse(StringIteratorRange(begin, itrange.end));
		
		if (!end.isOk() || i < this->minCount)
			return ParseResult(ParseResult::Status::Fatal, end.details.withMessage((std::stringstream() << "Expected \"" << this->end << "\", got \"" << getCharsUntilEol(begin, itrange.end) << "\"").str()));

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(start.result.begin, end.result.end));
	}
}