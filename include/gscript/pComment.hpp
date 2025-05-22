#ifndef _h_gscript_parser_comment
#define _h_gscript_parser_comment

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"

/// <summary>Find either line or block comment and increment given iterator</summary>
/// <param name="itrange">iterator range to scan in</param>
/// <param name="outitrange">iterator to increment</param>
#define COMMENT(itrange, outit, COMMENT_LENGTH) { \
	ParseResult commentResult = (ParserComment()).parse(itrange); \
	if (commentResult.isOk()) { \
		unsigned int _comment_length = commentResult.getLength(); \
		outit += _comment_length; \
		COMMENT_LENGTH = _comment_length; \
	} \
}

#define COMMENT_RESULT(itrange, commentLength) itrange.begin, itrange.begin + commentLength

#define COMMENT_DEF() COMMENT(itrange, outitrange.begin)

#define COMMENT_SHIFT(_RES, _OUT) _OUT += _RES.getLength();

#define COMMENT_SHIFT_E(_RES, _OUT) else _OUT += _RES.getLength();

#define COMMENT_SHIFT_COND_E(_RES, _OUT, _ELSE) \
	_ELSE if (_RES.isComment()) COMMENT_SHIFT(_RES, _OUT)

#define COMMENT_SHIFT_COND(_RES, _OUT) COMMENT_SHIFT_COND_E(_RES, _OUT, )

namespace gscript
{
	class ParserComment : public ParserEntity
	{
	public:
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif