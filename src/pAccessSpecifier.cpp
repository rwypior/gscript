#include <iostream>

#include "pAccessSpecifier.hpp"
#include "utilParserWord.hpp"
#include "compileException.hpp"

namespace gscript
{
	const char *ParserAccessSpecifier::KW_M_AS_PUBLIC = "public";
	const char *ParserAccessSpecifier::KW_M_AS_PRIVATE = "private";
	const char *ParserAccessSpecifier::KW_M_AS_PROTECTED = "protected";

	const char *ParserAccessSpecifier::KW_M_CONST = "const";

	const char *ParserAccessSpecifier::KW_M_STATIC = "static";

	const char *ParserAccessSpecifier::KW_M_VIRTUAL = "virtual";

	ParseResult ParserAccessSpecifier::parse(StringIteratorRange itrange)
	{
		std::string::iterator it = itrange.begin;

		bool anyGood = true;
		while (anyGood)
		{
			anyGood = false;

			ParseResult rpublic = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PUBLIC);
			if (rpublic.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC);
				it = rpublic.result.end;
				anyGood = true;
			}

			ParseResult rprotected = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PROTECTED);
			if (rprotected.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_ACCESS_PROTECTED);
				it = rprotected.result.end;
				anyGood = true;
			}

			ParseResult rprivate = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PRIVATE);
			if (rprivate.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_ACCESS_PRIVATE);
				it = rprivate.result.end;
				anyGood = true;
			}

			ParseResult rconst = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_CONST);
			if (rconst.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_CONST);
				it = rconst.result.end;
				anyGood = true;
			}

			ParseResult rstatic = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_STATIC);
			if (rstatic.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_STATIC);
				it = rstatic.result.end;
				anyGood = true;
			}

			ParseResult rvirtual = Util::Word::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_VIRTUAL);
			if (rvirtual.isOk())
			{
				this->modifier |= static_cast<int>(MODIFIER_T::M_VIRTUAL);
				it = rvirtual.result.end;
				anyGood = true;
			}
		}

		if (
			(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PRIVATE) && this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC)) ||
			(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PRIVATE) && this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PROTECTED)) ||
			(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PROTECTED) && this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC))
			)
			throw CompileException("Only one of PRIVATE, PROTECTED or PUBLIC modifiers might be in use");

		if (
			!(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC)) &&
			!(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PROTECTED)) &&
			!(this->modifier & static_cast<int>(MODIFIER_T::M_ACCESS_PRIVATE))
			)
			this->modifier |= static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC);

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it + 1));
	}

	int ParserAccessSpecifier::getModifier() const
	{
		return this->modifier;
	}
}