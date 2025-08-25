#include "parser/pClass.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pBlockStart.hpp"
#include "parser/pBlockEnd.hpp"
#include "parser/pInherit.hpp"
#include "parser/pMethod.hpp"
#include "parser/pFieldDeclaration.hpp"
#include "parser/pConstructor.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserClass::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserClass::KW_CLASS);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult name = (ParserNameSpecifier()).parse(StringIteratorRange(parentResult.result.end + 1, itrange.end));

		if (!name.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		this->name = name.getWord();

		ParseResult inherit = (ParserInherit()).parse(StringIteratorRange(name.result.end + 1, itrange.end));
		ParseResult blockBegin = (ParserBlockStart()).parse(StringIteratorRange(inherit.isOk() ? inherit.result.end + 1 : name.result.end + 1, itrange.end));

		if (inherit.isOk())
			this->base = inherit.getWord();

		if (!blockBegin.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		bool anyMatched = false;
		auto begin = blockBegin.result.end + 1;

		do
		{
			ParserConstructor pconstructor(*this);
			ParseResult pconstructorres = pconstructor.parse(StringIteratorRange(begin, itrange.end));
			if (pconstructorres.isOk())
			{
				this->methods.push_back(pconstructor);
				begin = pconstructorres.result.end;
			}

			ParserMethod pmethod;
			ParseResult pmethodres = pmethod.parse(StringIteratorRange(begin, itrange.end));
			if (pmethodres.isOk())
			{
				this->methods.push_back(pmethod);
				begin = pmethodres.result.end;
			}

			ParserFieldDeclaration pfield = ParserFieldDeclaration();
			ParseResult pfieldres = pfield.parse(StringIteratorRange(begin, itrange.end));
			if (pfieldres.isOk())
			{
				this->fields.push_back(pfield);
				begin = pfieldres.result.end;
			}

			anyMatched = pmethodres.isOk() || pfieldres.isOk() || pconstructorres.isOk();
		} while (anyMatched);

		ParseResult blockEnd = (ParserBlockEnd()).parse(StringIteratorRange(begin, itrange.end));

		if (!blockEnd.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, blockEnd.result.end));
	}
}