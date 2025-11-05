#include "gscript/parser/pAccessSpecifier.hpp"
#include "gscript/parser/pWord.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/compileException.hpp"
#include "gscript/StringUtils.hpp"

namespace gscript
{
	ParserAccessSpecifier::ParserAccessSpecifier(Modifier defaultModifier)
		: modifier(defaultModifier)
	{
	}

	ParseResult ParserAccessSpecifier::parse(StringIteratorRange itrange)
	{
		auto it = itrange.begin;

		size_t newlines = skipWhitespaces(it, itrange.end);

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(itrange.begin, it));

		bool anyGood = true;
		while (anyGood)
		{
			anyGood = false;

			it = parseComment(it, itrange.end);
			ParseResult rpublic = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordPublic);
			if (rpublic.isOk())
			{
				this->modifier |= Modifier::AccessPublic;
				it = rpublic.result.end;
				anyGood = true;
			}

			it = parseComment(it, itrange.end);
			ParseResult rprotected = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordProtected);
			if (rprotected.isOk())
			{
				this->modifier |= Modifier::AccessProtected;
				it = rprotected.result.end;
				anyGood = true;
			}

			it = parseComment(it, itrange.end);
			ParseResult rprivate = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordPrivate);
			if (rprivate.isOk())
			{
				this->modifier |= Modifier::AccessPrivate;
				it = rprivate.result.end;
				anyGood = true;
			}

			it = parseComment(it, itrange.end);
			ParseResult rconst = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordConst);
			if (rconst.isOk())
			{
				this->modifier |= Modifier::Const;
				it = rconst.result.end;
				anyGood = true;
			}

			it = parseComment(it, itrange.end);
			ParseResult rstatic = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordStatic);
			if (rstatic.isOk())
			{
				this->modifier |= Modifier::Static;
				it = rstatic.result.end;
				anyGood = true;
			}

			it = parseComment(it, itrange.end);
			ParseResult rvirtual = ParserWord::parse(StringIteratorRange(it, itrange.end), ParserAccessSpecifier::keywordVirtual);
			if (rvirtual.isOk())
			{
				this->modifier |= Modifier::Virtual;
				it = rvirtual.result.end;
				anyGood = true;
			}
		}

		if (
			(this->modifier & Modifier::AccessPrivate && this->modifier & Modifier::AccessPublic) ||
			(this->modifier & Modifier::AccessPrivate && this->modifier & Modifier::AccessProtected) ||
			(this->modifier & Modifier::AccessProtected && this->modifier & Modifier::AccessPublic)
			)
			return ParseResult(ParseResult::Status::Fatal, { itrange, "Access specifiers \"private\", \"protected\" and \"public\" must not be mixed" });

		if (
			(this->modifier & Modifier::Static && this->modifier & Modifier::Virtual)
			)
			return ParseResult(ParseResult::Status::Fatal, { itrange, "Access specifiers \"static\" and \"virtual\" must not be mixed" });

		if (this->modifier == Modifier::None)
			return ParseResult(ParseResult::Status::Fatal, { itrange, "Expected any of: public, private, protected, const, static, virtual; got \"" + getCharsUntilEol(it, itrange.end) + "\"" });

		if (
			!(this->modifier & Modifier::AccessPublic) &&
			!(this->modifier & Modifier::AccessProtected) &&
			!(this->modifier & Modifier::AccessPrivate)
			)
			this->modifier |= Modifier::AccessPublic;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(itrange.begin, it));
	}

	Modifier ParserAccessSpecifier::getModifier() const
	{
		return this->modifier;
	}
}