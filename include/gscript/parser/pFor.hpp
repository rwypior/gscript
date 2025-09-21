#ifndef _h_gscript_parser_for
#define _h_gscript_parser_for

#include "pEntity.hpp"
#include "pForArglist.hpp"
#include "pBlock.hpp"
#include "IteratorRange.hpp"

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