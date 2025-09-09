#ifndef _h_gscript_parser_literal
#define _h_gscript_parser_literal

#include "pEntity.hpp"
#include "defs.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserLiteral : public ParserEntity
	{
	public:
		VALUE_TYPE_T type = VALUE_TYPE_T::VT_VOID;
		std::string value;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif