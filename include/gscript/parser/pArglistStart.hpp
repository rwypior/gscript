#ifndef _h_gscript_parser_arglist_start
#define _h_gscript_parser_arglist_start

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserArglistStart : public ParserEntity
	{
	public:
		static constexpr char keycharArglistStart = '(';

		const char blockStart = keycharArglistStart;

		ParserArglistStart(char ch = ParserArglistStart::keycharArglistStart)
			: blockStart(ch)
		{
		}

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif