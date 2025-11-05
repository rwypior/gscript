#include "gscript/parser/pBlockBody.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/parser/pTypeSpecifier.hpp"
#include "gscript/parser/pBlockStart.hpp"
#include "gscript/parser/pBlockEnd.hpp"
#include "gscript/parser/pArglist.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pIf.hpp"
#include "gscript/parser/pWhile.hpp"
#include "gscript/parser/pFor.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	ParserBlockBody::ParserBlockBody(unsigned int maxEntries)
		:maxEntries(maxEntries)
	{
	}

	void ParserBlockBody::setMinEntries(unsigned int minEntries)
	{
		this->minEntries = minEntries;
	}

	void ParserBlockBody::setMaxEntries(unsigned int maxEntries)
	{
		this->maxEntries = maxEntries;
	}

	ParseResult ParserBlockBody::parse(StringIteratorRange itrange)
	{
		auto endPosition = itrange.begin;
		bool anyGood = false;
		unsigned int processedEntries = 0;

		do
		{
			endPosition = parseComment(endPosition, itrange.end);
			anyGood = false;

			ParserIf pif;
			ParseResult pifres = pif.parse(StringIteratorRange(endPosition, itrange.end));
			if (pifres.isOk())
			{
				endPosition = pifres.result.end;
				this->statements.push_back(std::make_shared<ParserIf>(std::move(pif)));
				anyGood = true;
				continue;
			}
			else if (pifres.isFatal())
				return pifres;

			ParserWhile pwhile;
			ParseResult pwhileres = pwhile.parse(StringIteratorRange(endPosition, itrange.end));
			if (pwhileres.isOk())
			{
				endPosition = pwhileres.result.end;
				this->statements.push_back(std::make_shared<ParserWhile>(std::move(pwhile)));
				anyGood = true;
				continue;
			}
			else if (pwhileres.isFatal())
				return pwhileres;

			ParserFor pfor;
			ParseResult pforres = pfor.parse(StringIteratorRange(endPosition, itrange.end));
			if (pforres.isOk())
			{
				endPosition = pforres.result.end;
				this->statements.push_back(std::make_shared<ParserFor>(std::move(pfor)));
				anyGood = true;
				continue;
			}
			else if (pforres.isFatal())
				return pforres;

			ParserReturn preturn;
			ParseResult preturnres = preturn.parse(StringIteratorRange(endPosition, itrange.end));
			if (preturnres.isOk())
			{
				endPosition = preturnres.result.end;
				this->statements.push_back(std::make_shared<ParserReturn>(std::move(preturn)));
				anyGood = true;
				continue;
			}
			else if (preturnres.isFatal())
				return preturnres;

			ParserVarDeclaration pvar;
			ParseResult pvarres = pvar.parse(StringIteratorRange(endPosition, itrange.end));
			if (pvarres.isOk())
			{
				endPosition = pvarres.result.end;
				this->statements.push_back(std::make_shared<ParserVarDeclaration>(std::move(pvar)));
				anyGood = true;
				continue;
			}
			else if (pvarres.isFatal())
				return pvarres;

			ParserStatement pstatement(false);
			ParseResult pstatementres = pstatement.parse(StringIteratorRange(endPosition, itrange.end));
			if (pstatementres.isOk())
			{
				endPosition = pstatementres.result.end;
				this->statements.push_back(std::make_shared<ParserStatement>(std::move(pstatement)));
				anyGood = true;
				continue;
			}
			else if (pstatementres.isFatal())
				return pstatementres;
		} while (anyGood && (!this->maxEntries || ++processedEntries < this->maxEntries));

		if (!anyGood && this->minEntries > 0)
		{
			if (this->maxEntries == 1)
				return ParseResult(ParseResult::Status::Fatal, { itrange, "Expected statement" });

			return ParseResult(ParseResult::Status::Fatal, { itrange, "Expected body block" });
		}

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(itrange.begin, endPosition));
	}
}