#ifndef _h_gscript_parser_inherit
#define _h_gscript_parser_inherit

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserInherit : public ParserEntity
	{
	public:
		static constexpr char keycharInherit = ':';

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif