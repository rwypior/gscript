#ifndef _h_gscript_parser_arglist
#define _h_gscript_parser_arglist

#include "pEntity.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pVarDeclaration.hpp"
#include "pListSeparator.hpp"
#include "IteratorRange.hpp"

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