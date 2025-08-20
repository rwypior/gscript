#include "parser/pBlockBody.hpp"
#include "parser/pFunction.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pTypeSpecifier.hpp"
#include "parser/pBlockStart.hpp"
#include "parser/pBlockEnd.hpp"
#include "parser/pArglist.hpp"
#include "parser/pVarDeclaration.hpp"
#include "parser/pStatement.hpp"
#include "parser/pReturn.hpp"
#include "parser/pIf.hpp"
#include "parser/pWhile.hpp"
#include "parser/pFor.hpp"
#include "parser/pComment.hpp"
#include "IteratorRange.hpp"

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
		auto endPosition = itrange.begin;
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
				this->statements.push_back(std::make_shared<ParserStatement>(std::move(pstatement)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pstatementres, endPosition);

			ParserVarDeclaration pvar;
			ParseResult pvarres = pvar.parse(StringIteratorRange(endPosition, itrange.end));
			if (pvarres.isOk())
			{
				endPosition = pvarres.result.end;
				this->statements.push_back(std::make_shared<ParserVarDeclaration>(std::move(pvar)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pvarres, endPosition);

			ParserIf pif;
			ParseResult pifres = pif.parse(StringIteratorRange(endPosition, itrange.end));
			if (pifres.isOk())
			{
				endPosition = pifres.result.end;
				this->statements.push_back(std::make_shared<ParserIf>(std::move(pif)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pifres, endPosition);

			ParserWhile pwhile;
			ParseResult pwhileres = pwhile.parse(StringIteratorRange(endPosition, itrange.end));
			if (pwhileres.isOk())
			{
				endPosition = pwhileres.result.end;
				this->statements.push_back(std::make_shared<ParserWhile>(std::move(pwhile)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pwhileres, endPosition);

			ParserFor pfor;
			ParseResult pforres = pfor.parse(StringIteratorRange(endPosition, itrange.end));
			if (pforres.isOk())
			{
				endPosition = pforres.result.end;
				this->statements.push_back(std::make_shared<ParserFor>(std::move(pfor)));
				anyGood = true;
				continue;
			}
			COMMENT_SHIFT_E(pforres, endPosition);

			ParserReturn preturn;
			ParseResult preturnres = preturn.parse(StringIteratorRange(endPosition, itrange.end));
			if (preturnres.isOk())
			{
				endPosition = preturnres.result.end;
				this->statements.push_back(std::make_shared<ParserReturn>(std::move(preturn)));
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

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, endPosition));
	}
}