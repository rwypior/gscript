#ifndef _h_parser_while
#define _h_parser_while

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"

namespace gscript
{
	class ParserWhile : public ParserEntity
	{
	public:
		static const char *KW_WHILE;

		ParserWhile();

		ParserBlock body;
		ParserCallArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif