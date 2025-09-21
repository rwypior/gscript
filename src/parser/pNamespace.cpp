#include "parser/pNamespace.hpp"
#include "parser/pFunction.hpp"
#include "parser/pClass.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pImportDirective.hpp"
#include "parser/pChar.hpp"
#include "parser/pWord.hpp"
#include "parser/pComment.hpp"
#include "StringUtils.hpp"
#include "compileException.hpp"

#include <iostream>
#include <vector>

namespace gscript
{
	ParserNamespace::ParserNamespace(NamespaceType type)
		: type(type)
	{
	}

	ParseResult ParserNamespace::parse(StringIteratorRange itrange)
	{
		auto beginPosition = itrange.end;
		auto endPosition = itrange.begin;

		if (this->type & NamespaceType::Named)
		{
			endPosition = parseComment(endPosition, itrange.end);
			ParseResult nsResult = ParserWord::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::keywordNamespace);

			if (!nsResult.isOk())
				return nsResult;

			ParserNameSpecifier nsName;
			nsResult.result.end = parseComment(nsResult.result.end, itrange.end);
			ParseResult nsNameResult = nsName.parse(StringIteratorRange(nsResult.result.end, itrange.end));

			if (!nsNameResult.isOk())
				return nsNameResult.as(ParseResult::Status::Fatal);

			this->name = nsName.name;

			beginPosition = nsResult.result.begin;
			endPosition = nsNameResult.result.end;
		}

		if (this->type & NamespaceType::Enclosed)
		{
			endPosition = parseComment(endPosition, itrange.end);
			ParseResult enclosureResult = ParserChar::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::keycharEnclosureBegin);

			if (!enclosureResult.isOk())
				return enclosureResult.as(ParseResult::Status::Fatal);

			endPosition = enclosureResult.result.end;

			if (enclosureResult.result.begin < beginPosition)
				beginPosition = enclosureResult.result.begin;
		}

		bool anyGood = false;

		do
		{
			anyGood = false;
			endPosition = parseComment(endPosition, itrange.end);
			StringIteratorRange range(endPosition, itrange.end);

			if (this->type & NamespaceType::Main)
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

			ParserNamespace pnamespace(NamespaceType::Regular);
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

		if (this->type & NamespaceType::Enclosed)
		{
			endPosition = parseComment(endPosition, itrange.end);
			ParseResult enclosureResult = ParserChar::parse(StringIteratorRange(endPosition, itrange.end), ParserNamespace::keycharEnclosureEnd);

			if (!enclosureResult.isOk())
				return enclosureResult.as(ParseResult::Status::Fatal);

			endPosition = enclosureResult.result.end;
		}

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(beginPosition, endPosition));
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