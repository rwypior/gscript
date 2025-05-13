#ifndef _h_parser_abstract_special
#define _h_parser_abstract_special

#include "pEntity.hpp"
#include "pSpecialDirective.hpp"

#include <string>

namespace gscript
{
	class ParserAbstractSpecial : public ParserSpecialDirective
	{
	public:
		static const char *DIRECTIVE_ABSTRACT;

		ParserAbstractSpecial();

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif