#ifndef _h_gscript_parser_linecomment
#define _h_gscript_parser_linecomment

#include "pEntity.hpp"

#include <string>

namespace gscript
{
	class ParserLineComment : public ParserEntity
	{
	public:
		const char *CHR_LINE_COMMENT = "//";

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif