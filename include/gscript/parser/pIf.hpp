#ifndef _h_gscript_parser_if
#define _h_gscript_parser_if

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserIf : public ParserEntity
	{
	public:
		const char *KW_IF = "if";

		ParserIf();

		ParserBlock body;
		ParserCallArglist arglist;
		ParserElse pelse;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif