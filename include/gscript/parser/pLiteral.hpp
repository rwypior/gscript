#ifndef _h_gscript_parser_literal
#define _h_gscript_parser_literal

#include "gscript/parser/pEntity.hpp"
#include "gscript/defs.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserLiteral : public ParserEntity
	{
	public:
		ValueType type = ValueType::Void;
		std::string value;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif