#ifndef _h_parser_blockcomment
#define _h_parser_blockcomment

#include "pEntity.hpp"

#include <string>

namespace gscript
{
	class ParserBlockComment : public ParserEntity
	{
	public:
		const char *CHR_BLOCK_COMMENT_BEGIN = "/*";
		const char *CHR_BLOCK_COMMENT_END = "*/";

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif