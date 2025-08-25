#include "parser/pNamespace.hpp"
#include "parser/pFunction.hpp"
#include "parser/pClass.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pImportDirective.hpp"
#include "StringUtils.hpp"
#include "parser/pChar.hpp"
#include "parser/pWord.hpp"
#include "compileException.hpp"

#include <iostream>
#include <vector>

namespace gscript
{
	const char *ParserNamespace::KW_NAMESPACE = "namespace";
	const char ParserNamespace::KW_ENCLOSURE_BEGIN = '{';
	const char ParserNamespace::KW_ENCLOSURE_END = '}';

	ParserNamespace::ParserNamespace(NAMESPACE_TYPE_T type)
		: type(type)
	{
	}

	ParseResult ParserNamespace::parse(StringIteratorRange itrange)
	{
		auto beginPosition = itrange.end;
		auto endPosition = itrange.begin;

		if (static_cast<BITFLAG_T>(this->type) & static_cast<BITFLAG_T>(NAMESPACE_TYPE_T::NT_NAMED))
		{
			ParseResult nsResult = ParserWord::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::KW_NAMESPACE);

			if (!nsResult.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL);

			ParserNameSpecifier nsName;
			ParseResult nsNameResult = nsName.parse(StringIteratorRange(nsResult.result.end, itrange.end));

			if (!nsNameResult.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL);

			this->name = nsName.name;

			beginPosition = nsResult.result.begin;
			endPosition = nsNameResult.result.end;
		}

		if (static_cast<BITFLAG_T>(this->type) & static_cast<BITFLAG_T>(NAMESPACE_TYPE_T::NT_ENCLOSED))
		{
			ParseResult enclosureResult = ParserChar::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::KW_ENCLOSURE_BEGIN);

			if (!enclosureResult.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL);

			endPosition = enclosureResult.result.end;

			if (enclosureResult.result.begin < beginPosition)
				beginPosition = enclosureResult.result.begin;
		}

		bool anyGood = false;

		do
		{
			anyGood = false;
			StringIteratorRange range(endPosition, itrange.end);

			if (static_cast<BITFLAG_T>(this->type) & static_cast<BITFLAG_T>(NAMESPACE_TYPE_T::NT_MAIN))
			{
				ParserImportDirective pdimport;
				ParseResult pdimportres = pdimport.parse(range);
				if (pdimportres.isOk())
				{
					endPosition = pdimportres.result.end;

					if (static_cast<int>(pdimport.type) & static_cast<int>(ParserImportDirective::IMPORT_TYPE_T::IT_FILE))
						this->imports.push_back(pdimport.filename);
					else if (static_cast<int>(pdimport.type) & static_cast<int>(ParserImportDirective::IMPORT_TYPE_T::IT_EXTENSION))
						this->extensions.push_back(pdimport.filename);
					else
						throw CompileException("Invalid import directive");

					anyGood = true;

					if (pdimportres.result.begin < beginPosition)
						beginPosition = pdimportres.result.begin;

					continue;
				}
			}

			ParserNamespace pnamespace(NAMESPACE_TYPE_T::NT_REGULAR);
			ParseResult pnamespaceres = pnamespace.parse(range);
			if (pnamespaceres.isOk())
			{
				endPosition = pnamespaceres.result.end;
				this->namespaces.push_back(pnamespace);
				anyGood = true;

				if (pnamespaceres.result.begin < beginPosition)
					beginPosition = pnamespaceres.result.begin;

				continue;
			}

			ParserClass pclass;
			ParseResult pclassres = pclass.parse(range);
			if (pclassres.isOk())
			{
				endPosition = pclassres.result.end;
				this->classes.push_back(pclass);
				anyGood = true;

				if (pclassres.result.begin < beginPosition)
					beginPosition = pclassres.result.begin;

				continue;
			}

			ParserFunction pfunc;
			ParseResult pfuncres = pfunc.parse(range);
			if (pfuncres.isOk())
			{
				endPosition = pfuncres.result.end;
				this->functions.push_back(std::move(pfunc));
				anyGood = true;

				if (pfuncres.result.begin < beginPosition)
					beginPosition = pfuncres.result.begin;

				continue;
			}
		} while ((endPosition < itrange.end) && anyGood);

		if (static_cast<BITFLAG_T>(this->type) & static_cast<BITFLAG_T>(NAMESPACE_TYPE_T::NT_ENCLOSED))
		{
			ParseResult enclosureResult = ParserChar::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::KW_ENCLOSURE_END);

			if (!enclosureResult.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL);

			endPosition = enclosureResult.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(beginPosition, endPosition));
	}

	bool ParserNamespace::findClass(const std::string &name, ParserClass &pClass)
	{
		for (CLASS_CONTAINER_T::iterator it = this->classes.begin(); it != this->classes.end(); ++it)
		{
			if (it->name == name)
			{
				pClass = *it;
				return true;
			}
		}

		return false;
	}
}