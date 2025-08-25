#include "parser/pFor.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pArglistEnd.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	const char *ParserFor::KW_FOR = "for";

	ParserFor::ParserFor()
	{ }

	ParseResult ParserFor::parse(StringIteratorRange itrange)
	{
		ParseResult parentResult = ParserWord::parse(itrange, ParserFor::KW_FOR);
		if (parentResult.status != ParseResult::STATUS_T::S_OK)
			return parentResult;

		ParseResult arglistres = this->arglist.parse(StringIteratorRange(parentResult.result.end, itrange.end));
		if (!arglistres.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { arglistres, "Expected argument list" });

		auto begin = arglistres.result.end;

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin, itrange.end));
		if (bodyres.isOk())
			begin = bodyres.result.end;
		else
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { arglistres, "Expected body block" });

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(parentResult.result.begin, begin));
	}
}