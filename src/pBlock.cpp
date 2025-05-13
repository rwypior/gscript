#include "pBlock.hpp"
#include "pBlockBody.hpp"
#include "pBlockStart.hpp"
#include "pBlockEnd.hpp"

#include <iostream>

namespace gscript
{
	ParseResult ParserBlock::parse(StringIteratorRange itrange)
	{
		ParseResult begin = (ParserBlockStart()).parse(StringIteratorRange(itrange.begin, itrange.end));
		if (!begin.isOk())
		{
			this->body.setMaxEntries(1);
			ParseResult bodyres = this->body.parse(StringIteratorRange(itrange.begin, itrange.end));

			return bodyres;
		}

		ParseResult bodyres = this->body.parse(StringIteratorRange(begin.result.end, itrange.end));
		if (!bodyres.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		ParseResult end = (ParserBlockEnd()).parse(StringIteratorRange(bodyres.result.end, itrange.end));
		if (!end.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL);

		return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(begin.result.begin, end.result.end));
	}
}