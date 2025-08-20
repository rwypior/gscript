#include "parser/pNew.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pFuncCall.hpp"
#include "utilParserWord.hpp"

namespace gscript
{
	ParseResult ParserNew::parse(StringIteratorRange itrange)
	{
		ParseResult rnew = Util::Word::parse(itrange, ParserNew::KW_NEW);
		if (!rnew.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		ParseResult rfcall = ParserFuncCall::parse(StringIteratorRange(rnew.result.end + 1, itrange.end));
		if (!rfcall.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		this->constructorName = this->name;

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(rnew.result.begin, rfcall.result.end));
	}
}