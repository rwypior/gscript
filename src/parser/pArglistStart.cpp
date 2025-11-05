#include "gscript/parser/pChar.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pComment.hpp"

namespace gscript
{
	ParseResult ParserArglistStart::parse(StringIteratorRange itrange)
	{
		if (itrange.begin >= itrange.end)
			return ParseResult(ParseResult::Status::Invalid, { itrange, "Expected argument list" });

		itrange.begin = parseComment(itrange.begin, itrange.end);
		return ParserChar::parse(StringIteratorRange(itrange.begin, itrange.end), this->blockStart);
	}
}