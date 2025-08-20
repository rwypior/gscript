#ifndef _h_gscript_parser_arglist_start
#define _h_gscript_parser_arglist_start

#include "pEntity.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserArglistStart : public ParserEntity
	{
	public:
		static const char C_ARGLIST_START;

		ParserArglistStart(char ch = ParserArglistStart::C_ARGLIST_START)
			: KW_BLOCKSTART(ch)
		{
		}

		const char KW_BLOCKSTART = 0;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif