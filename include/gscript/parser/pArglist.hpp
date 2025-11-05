#ifndef _h_gscript_parser_arglist
#define _h_gscript_parser_arglist

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/IteratorRange.hpp"

#include <vector>

namespace gscript
{
	class ParserArglist : public ParserEntity
	{
	public:
		std::vector<ParserVarDeclaration> parameters;

		ParserArglist(char start = ParserArglistStart::keycharArglistStart, char end = ParserArglistEnd::keycharArglistEnd);
		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		char arglistStart = 0;
		char arglistEnd = 0;
	};
}

#endif