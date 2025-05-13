#include "pBlockBody.hpp"
#include "pFunction.hpp"
#include "pNameSpecifier.hpp"
#include "pTypeSpecifier.hpp"
#include "pBlockStart.hpp"
#include "pBlockEnd.hpp"
#include "pArglist.hpp"
#include "pVarDeclaration.hpp"
#include "pStatement.hpp"
#include "pReturn.hpp"
#include "pIf.hpp"
#include "pWhile.hpp"
#include "pFor.hpp"
#include "pComment.hpp"

#include <iostream>

namespace gscript
{
	ParserBlockBody::ParserBlockBody(unsigned int maxEntries)
		:maxEntries(maxEntries)
	{
	}

	void ParserBlockBody::setMaxEntries(unsigned int maxEntries)
	{
		this->maxEntries = maxEntries;
	}

	ParseResult ParserBlockBody::parse(StringIteratorRange itrange)
	{
		std::string::iterator endPosition = itrange.begin;
		bool anyGood = false;
		unsigned int processedEntries = 0;

		do
		{
			anyGood = false;

			ParserStatement pstatement(false);
			ParseResult pstatementres = pstatement.parse(StringIteratorRange(endPosition, itrange.end));
			if (pstatementres.isOk())
			{
				endPosition = pstatementres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserStatement(pstatement)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pstatementres, endPosition);

			ParserVarDeclaration pvar;
			ParseResult pvarres = pvar.parse(StringIteratorRange(endPosition, itrange.end));
			if (pvarres.isOk())
			{
				endPosition = pvarres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserVarDeclaration(pvar)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pvarres, endPosition);

			ParserIf pif;
			ParseResult pifres = pif.parse(StringIteratorRange(endPosition, itrange.end));
			if (pifres.isOk())
			{
				endPosition = pifres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserIf(pif)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pifres, endPosition);

			ParserWhile pwhile;
			ParseResult pwhileres = pwhile.parse(StringIteratorRange(endPosition, itrange.end));
			if (pwhileres.isOk())
			{
				endPosition = pwhileres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserWhile(pwhile)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pwhileres, endPosition);

			ParserFor pfor;
			ParseResult pforres = pfor.parse(StringIteratorRange(endPosition, itrange.end));
			if (pforres.isOk())
			{
				endPosition = pforres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserFor(pfor)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pforres, endPosition);

			ParserReturn preturn;
			ParseResult preturnres = preturn.parse(StringIteratorRange(endPosition, itrange.end));
			if (preturnres.isOk())
			{
				endPosition = preturnres.result.end;
				this->statements.push_back(std::shared_ptr<ParserEntity>(new ParserReturn(preturn)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(preturnres, endPosition);

			/*anyGood =
				pvarres.isOk() ||
				pstatementres.isOk() ||
				preturnres.isOk() ||
				pifres.isOk() ||
				pwhileres.isOk();*/
		} while (anyGood && (!this->maxEntries || ++processedEntries < this->maxEntries));

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(itrange.begin, endPosition));
	}
}