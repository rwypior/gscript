#ifndef _h_gscript_parser_funccall
#define _h_gscript_parser_funccall

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/EntityPath.hpp"
#include "gscript/IteratorRange.hpp"

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