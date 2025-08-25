#include "parser/pChar.hpp"
#include "parser/pArglistStart.hpp"
#include "parser/pComment.hpp"

namespace gscript
{
	const char ParserArglistStart::C_ARGLIST_START = '(';

	ParseResult ParserArglistStart::parse(StringIteratorRange itrange)
	{
		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		//if (itrange.begin + 1 >= itrange.end)
		if (itrange.begin >= itrange.end)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));

		return ParserChar::parse(StringIteratorRange(itrange.begin, itrange.end), this->KW_BLOCKSTART);
	}
}