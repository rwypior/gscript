#ifndef _h_gscript_parser_linecomment
#define _h_gscript_parser_linecomment

#include "pEntity.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserLineComment : public ParserEntity
	{
	public:
		static constexpr char keywordLineComment[] = "//";

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif