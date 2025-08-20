#ifndef _h_gscript_parser_funccall
#define _h_gscript_parser_funccall

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "pVar.hpp"
#include "EntityPath.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserFuncCall : public ParserEntity
	{
	public:
		EntityPath name;
		ParserCallArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif