#ifndef _h_gscript_parser_fielddeclaration
#define _h_gscript_parser_fielddeclaration

#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pAccessSpecifier.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserFieldDeclaration : public ParserVarDeclaration
	{
	public:
		ParserAccessSpecifier accessSpecifier;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif