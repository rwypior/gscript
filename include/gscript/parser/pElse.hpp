#ifndef _h_gscript_parser_else
#define _h_gscript_parser_else

#include "pEntity.hpp"
#include "pBlock.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserIf;

	class ParserElse : public ParserEntity
	{
	public:
		const char *KW_ELSE = "else";

		ParserBlock body;
		ParserIf *pif = nullptr;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif