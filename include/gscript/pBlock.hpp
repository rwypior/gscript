#ifndef _h_parser_block
#define _h_parser_block

#include "pEntity.hpp"
#include "pBlockBody.hpp"

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