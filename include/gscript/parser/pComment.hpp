#ifndef _h_gscript_parser_comment
#define _h_gscript_parser_comment

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/ParseResult.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserComment : public ParserEntity
	{
	public:
		ParserComment(bool mergeLineComments = true);

		virtual ParseResult parse(StringIteratorRange itrange) override;

	private:
		bool mergeLineComments = true;
	};

	// Utility function to parse comment and return new itrator from which the parsing is supposed to be continued
	// or previous old begin iterator. The intended use for this function is as the following:
	//		begin = parseComment(begin, end);
	StringIteratorRange::ITERATOR_T parseComment(StringIteratorRange itrange);

	// Utility function to call parseComment(StringIteratorRange)
	StringIteratorRange::ITERATOR_T parseComment(StringIteratorRange::ITERATOR_T begin, StringIteratorRange::ITERATOR_T end);
}

#endif