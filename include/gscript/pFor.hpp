#ifndef _h_parser_for
#define _h_parser_for

#include "pEntity.hpp"
#include "pForArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"

namespace gscript
{
	class ParserFor : public ParserEntity
	{
	public:
		static const char *KW_FOR;

		ParserFor();

		ParserBlock body;
		ParserForArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif