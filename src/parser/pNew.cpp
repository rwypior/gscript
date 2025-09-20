#include "parser/pNew.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pFuncCall.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	ParseResult ParserNew::parse(StringIteratorRange itrange)
	{
		ParseResult rnew = ParserWord::parse(itrange, ParserNew::keywordNew);
		if (!rnew.isOk())
			return rnew;

		ParseResult rfcall = ParserFuncCall::parse(StringIteratorRange(rnew.result.end, itrange.end));
		if (!rfcall.isOk())
			return rfcall;

		this->constructorName = this->name;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(rnew.result.begin, rfcall.result.end));
	}
}