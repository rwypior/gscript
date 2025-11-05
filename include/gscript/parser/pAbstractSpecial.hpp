#ifndef _h_gscript_parser_abstract_special
#define _h_gscript_parser_abstract_special

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pSpecialDirective.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserAbstractSpecial : public ParserSpecialDirective
	{
	public:
		static constexpr char keywordAbstract[] = "abstract";

		ParserAbstractSpecial();

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif