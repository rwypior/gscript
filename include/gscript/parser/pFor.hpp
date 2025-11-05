#ifndef _h_gscript_parser_for
#define _h_gscript_parser_for

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pForArglist.hpp"
#include "gscript/parser/pBlock.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserFor : public ParserEntity
	{
	public:
		static constexpr char keywordFor[] = "for";

		ParserBlock body;
		ParserForArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif