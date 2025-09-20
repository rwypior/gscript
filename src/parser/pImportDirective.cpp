#include "parser/pImportDirective.hpp"
#include "parser/pChar.hpp"

#include <string>
#include <sstream>

namespace gscript
{
	ParserImportDirective::ParserImportDirective()
		: ParserControlDirective(ParserImportDirective::keywordImport)
	{
	}

	ParseResult ParserImportDirective::parse(StringIteratorRange itrange)
	{
		ParseResult controlResult = ParserControlDirective::parse(itrange);

		if (!controlResult.isOk())
			return controlResult;

		char enclosureEnd = ParserImportDirective::keycharEnclosureEnd;

		ParseResult beginResult = ParserChar::parse(StringIteratorRange(controlResult.result.end, itrange.end), ParserImportDirective::keycharEnclosureBegin);
		if (!beginResult.isOk())
		{
			beginResult = ParserChar::parse(StringIteratorRange(controlResult.result.end, itrange.end), ParserImportDirective::keycharExtensionEnclosureBegin);

			if (!beginResult.isOk())
				return beginResult;

			enclosureEnd = ParserImportDirective::keycharExtensionEnclosureEnd;
			this->type = IMPORT_TYPE_T::IT_EXTENSION;
		}
		else
			this->type = IMPORT_TYPE_T::IT_FILE;

		auto it = beginResult.result.end;

		bool endGood = false;
		for (; it != itrange.end; ++it)
		{
			if (*it == enclosureEnd)
			{
				endGood = true;
				break;
			}

			this->filename += *it;
		}

		if (endGood)
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(controlResult.result.begin, ++it));

		return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Missing import directive enclosure \"" << enclosureEnd << "\"").str() });
	}
}