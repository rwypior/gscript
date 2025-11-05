#ifndef _h_gscript_parser_blockcomment
#define _h_gscript_parser_blockcomment

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserBlockComment : public ParserEntity
	{
	public:
		static constexpr char keycharBlockCommentBegin[] = "/*";
		static constexpr char keycharBlockCommentEnd[] = "*/";

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif