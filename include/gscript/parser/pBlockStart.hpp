#ifndef _h_gscript_parse_blockstart
#define _h_gscript_parse_blockstart

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserBlockStart : public ParserEntity
	{
	public:
		static constexpr char keycharBlockStart = '{';

		const char blockStart = keycharBlockStart;

		ParserBlockStart(char ch = keycharBlockStart)
			: blockStart(ch)
		{
		}

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif