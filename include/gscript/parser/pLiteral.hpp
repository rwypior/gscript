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
		ValueType type = ValueType::Void;
		std::string value;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif