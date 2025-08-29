#include "parser/pAccessSpecifier.hpp"
#include "parser/pWord.hpp"
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
		auto it = itrange.begin;

		bool anyGood = true;
		while (anyGood)
		{
			anyGood = false;

			ParseResult rpublic = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PUBLIC);
			if (rpublic.isOk())
			{
				this->modifier |= MODIFIER_T::M_ACCESS_PUBLIC;
				it = rpublic.result.end;
				anyGood = true;
			}

			ParseResult rprotected = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PROTECTED);
			if (rprotected.isOk())
			{
				this->modifier |= MODIFIER_T::M_ACCESS_PROTECTED;
				it = rprotected.result.end;
				anyGood = true;
			}

			ParseResult rprivate = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_AS_PRIVATE);
			if (rprivate.isOk())
			{
				this->modifier |= MODIFIER_T::M_ACCESS_PRIVATE;
				it = rprivate.result.end;
				anyGood = true;
			}

			ParseResult rconst = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_CONST);
			if (rconst.isOk())
			{
				this->modifier |= MODIFIER_T::M_CONST;
				it = rconst.result.end;
				anyGood = true;
			}

			ParseResult rstatic = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_STATIC);
			if (rstatic.isOk())
			{
				this->modifier |= MODIFIER_T::M_STATIC;
				it = rstatic.result.end;
				anyGood = true;
			}

			ParseResult rvirtual = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::KW_M_VIRTUAL);
			if (rvirtual.isOk())
			{
				this->modifier |= MODIFIER_T::M_VIRTUAL;
				it = rvirtual.result.end;
				anyGood = true;
			}
		}

		if (
			(this->modifier & MODIFIER_T::M_ACCESS_PRIVATE && this->modifier & MODIFIER_T::M_ACCESS_PUBLIC) ||
			(this->modifier & MODIFIER_T::M_ACCESS_PRIVATE && this->modifier & MODIFIER_T::M_ACCESS_PROTECTED) ||
			(this->modifier & MODIFIER_T::M_ACCESS_PROTECTED && this->modifier & MODIFIER_T::M_ACCESS_PUBLIC)
			)
			throw CompileException("Only one of PRIVATE, PROTECTED or PUBLIC modifiers may be used");

		if (
			!(this->modifier & MODIFIER_T::M_ACCESS_PUBLIC) &&
			!(this->modifier & MODIFIER_T::M_ACCESS_PROTECTED) &&
			!(this->modifier & MODIFIER_T::M_ACCESS_PRIVATE)
			)
			this->modifier |= MODIFIER_T::M_ACCESS_PUBLIC;

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it));
	}

	MODIFIER_T ParserAccessSpecifier::getModifier() const
	{
		return this->modifier;
	}
}