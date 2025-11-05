#ifndef _h_gscript_parser_block
#define _h_gscript_parser_block

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pBlockBody.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserBlock : public ParserEntity
	{
	public:
		ParserBlockBody body;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif