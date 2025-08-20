#ifndef _h_gscript_parser_method
#define _h_gscript_parser_method

#include "pEntity.hpp"
#include "pFunction.hpp"
#include "pAccessSpecifier.hpp"
#include "IteratorRange.hpp"

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