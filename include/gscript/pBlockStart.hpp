#ifndef _h_parse_blockstart
#define _h_parse_blockstart

#include "pEntity.hpp"

namespace gscript
{
	class ParserBlockStart : public ParserEntity
	{
	public:
		const char KW_BLOCKSTART = '{';

		ParserBlockStart(char ch = '{')
			: KW_BLOCKSTART(ch)
		{
		}

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif