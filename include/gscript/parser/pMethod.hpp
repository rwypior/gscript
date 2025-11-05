#ifndef _h_gscript_parser_method
#define _h_gscript_parser_method

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pAccessSpecifier.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserMethod : public ParserFunction
	{
	public:
		ParserAccessSpecifier accessSpecifier;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif