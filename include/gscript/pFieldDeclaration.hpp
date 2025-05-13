#ifndef _h_parser_fielddeclaration
#define _h_parser_fielddeclaration

#include "pVarDeclaration.hpp"
#include "pAccessSpecifier.hpp"

#include <vector>

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