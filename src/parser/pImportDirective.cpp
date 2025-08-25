#include "parser/pImportDirective.hpp"
#include "parser/pChar.hpp"

#include <string>

namespace gscript
{
	const char *ParserImportDirective::DIRECTIVE_IMPORT = "import";

	const char ParserImportDirective::ENCLOSURE_BEGIN = '"';
	const char ParserImportDirective::ENCLOSURE_END = '"';

	const char ParserImportDirective::EXTENSION_ENCLOSURE_BEGIN = '<';
	const char ParserImportDirective::EXTENSION_ENCLOSURE_END = '>';

	/*ParserImportDirective::ParserImportDirective(char enclosureBegin, char enclosureEnd)
		:ParserControlDirective(ParserImportDirective::DIRECTIVE_IMPORT),
		enclosureBegin(enclosureBegin),
		enclosureEnd(enclosureEnd)
	{
	}*/

	ParserImportDirective::ParserImportDirective()
		:ParserControlDirective(ParserImportDirective::DIRECTIVE_IMPORT)
	{
	}

	ParseResult ParserImportDirective::parse(StringIteratorRange itrange)
	{
		ParseResult controlResult = ParserControlDirective::parse(itrange);

		if (!controlResult.isOk())
			return controlResult;

		char enclosureEnd = ParserImportDirective::ENCLOSURE_END;

		//ParseResult beginResult = Util::Char::parse(StringIteratorRange(controlResult.result.end, itrange.end), this->enclosureBegin);
		//if (beginResult.status != ParseResult::STATUS_T::S_OK)
			//return beginResult;

		ParseResult beginResult = ParserChar::parse(StringIteratorRange(controlResult.result.end, itrange.end), ParserImportDirective::ENCLOSURE_BEGIN);
		if (!beginResult.isOk())
		{
			beginResult = ParserChar::parse(StringIteratorRange(controlResult.result.end, itrange.end), ParserImportDirective::EXTENSION_ENCLOSURE_BEGIN);

			if (!beginResult.isOk())
				return beginResult;

			enclosureEnd = ParserImportDirective::EXTENSION_ENCLOSURE_END;
			this->type = IMPORT_TYPE_T::IT_EXTENSION;
		}
		else
			this->type = IMPORT_TYPE_T::IT_FILE;

		auto it = beginResult.result.end;

		bool endGood = false;
		for (; it != itrange.end; ++it)
		{
			//if (*it == this->enclosureEnd)
			if (*it == enclosureEnd)
			{
				endGood = true;
				break;
			}

			this->filename += *it;
		}

		if (endGood)
			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(controlResult.result.begin, ++it));

		return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
	}
}