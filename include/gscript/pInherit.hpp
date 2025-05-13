#ifndef _h_parser_inherit
#define _h_parser_inherit

#include "pEntity.hpp"

namespace gscript
{
	class ParserInherit : public ParserEntity
	{
	public:
		const char KW_INHERIT = ':';

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif