#ifndef _h_gscript_parser_inherit
#define _h_gscript_parser_inherit

#include "pEntity.hpp"
#include "IteratorRange.hpp"

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