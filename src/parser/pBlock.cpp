#include "parser/pBlock.hpp"
#include "parser/pBlockBody.hpp"
#include "parser/pBlockStart.hpp"
#include "parser/pBlockEnd.hpp"
#include "parser/pComment.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	ParseResult ParserBlock::parse(StringIteratorRange itrange)
	{
		itrange.begin = parseComment(itrange.begin, itrange.end);
		ParseResult begin = (ParserBlockStart()).parse(StringIteratorRange(itrange.begin, itrange.end));
		if (!begin.isOk())
		{
			this->body.setMinEntries(1);
			this->body.setMaxEntries(1);
			ParseResult bodyres = this->body.parse(StringIteratorRange(itrange.begin, itrange.end));

			return bodyres;
		}

		begin.result.end = parseComment(begin.result.end, itrange.end);
		ParseResult bodyres = this->body.parse(StringIteratorRange(begin.result.end, itrange.end));
		if (!bodyres.isOk())
			return bodyres;

		bodyres.result.end = parseComment(bodyres.result.end, itrange.end);
		ParseResult end = (ParserBlockEnd()).parse(StringIteratorRange(bodyres.result.end, itrange.end));
		if (!end.isOk())
			return end;

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(begin.result.begin, end.result.end));
	}
}