#ifndef _h_gscript_parser_if
#define _h_gscript_parser_if

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pBlock.hpp"
#include "gscript/parser/pElse.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserIf : public ParserEntity
	{
	public:
		static constexpr char keywordIf[] = "if";

		ParserIf();

		ParserBlock body;
		ParserCallArglist arglist;
		ParserElse pelse;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif