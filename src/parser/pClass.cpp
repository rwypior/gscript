#include "gscript/parser/pClass.hpp"
#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/parser/pBlockStart.hpp"
#include "gscript/parser/pBlockEnd.hpp"
#include "gscript/parser/pInherit.hpp"
#include "gscript/parser/pMethod.hpp"
#include "gscript/parser/pFieldDeclaration.hpp"
#include "gscript/parser/pConstructor.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/StringUtils.hpp"

namespace gscript
{
	ParseResult ParserClass::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserClass::keywordClass);
		if (parentResult.status != ParseResult::Status::Ok)
			return parentResult;

		ParseResult name = (ParserNameSpecifier()).parse(StringIteratorRange(parentResult.result.end + 1, itrange.end, itrange.getFile(), itrange.getLine()));

		if (!name.isOk())
			return ParseResult(ParseResult::Status::Invalid, StringIteratorRange());

		this->name = name.getWord();

		ParseResult inherit = (ParserInherit()).parse(
			StringIteratorRange(name.result.end + 1, itrange.end, itrange.getFile(), name.result.getLine()));
		ParseResult blockBegin = (ParserBlockStart()).parse(
			StringIteratorRange(inherit.isOk() ? inherit.result.end + 1 : name.result.end + 1, itrange.end, inherit.result.getFile(), inherit.result.getLine()));

		if (inherit.isOk())
			this->base = inherit.getWord();

		if (!blockBegin.isOk())
			return blockBegin;

		bool anyMatched = false;
		auto begin = blockBegin.result.end;

		size_t newlines = skipWhitespaces(begin, itrange.end);
		size_t line = blockBegin.result.getLine() + newlines;

		do
		{
			ParserConstructor pconstructor(*this);
			ParseResult pconstructorres = pconstructor.parse(StringIteratorRange(begin, itrange.end, blockBegin.result.getFile(), line));
			if (pconstructorres.isOk())
			{
				this->methods.push_back(pconstructor);
				begin = pconstructorres.result.end;
				line = pconstructorres.result.getLine();
			}

			ParserMethod pmethod;
			ParseResult pmethodres = pmethod.parse(StringIteratorRange(begin, itrange.end, blockBegin.result.getFile(), line));
			if (pmethodres.isOk())
			{
				this->methods.push_back(pmethod);
				begin = pmethodres.result.end;
				line = pmethodres.result.getLine();
			}

			ParserFieldDeclaration pfield = ParserFieldDeclaration();
			ParseResult pfieldres = pfield.parse(StringIteratorRange(begin, itrange.end, blockBegin.result.getFile(), line));
			if (pfieldres.isOk())
			{
				this->fields.push_back(pfield);
				begin = pfieldres.result.end;
				line = pfieldres.result.getLine();
			}

			anyMatched = pmethodres.isOk() || pfieldres.isOk() || pconstructorres.isOk();
		} while (anyMatched);

		ParseResult blockEnd = (ParserBlockEnd()).parse(StringIteratorRange(begin, itrange.end, blockBegin.result.getFile(), line));

		if (!blockEnd.isOk())
			return ParseResult(
				ParseResult::Status::Invalid,
				blockEnd.details.withMessage("Expected one of: constructor, method, field; got \"" + getCharsUntil(begin, itrange.end, '\n') + "\"")
			);

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(parentResult.result.begin, blockEnd.result.end));
	}
}