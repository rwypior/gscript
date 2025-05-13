#ifndef _h_parser_arglist
#define _h_parser_arglist

#include "pEntity.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pVarDeclaration.hpp"
#include "pListSeparator.hpp"

#include <vector>

namespace gscript
{
	class ParserArglist : public ParserEntity
	{
	protected:
		char arglistStart;
		char arglistEnd;

	public:
		typedef std::vector<ParserVarDeclaration> ARGLIST_PARAMS_T;

		ARGLIST_PARAMS_T parameters;

		ParserArglist(char start = ParserArglistStart::C_ARGLIST_START, char end = ParserArglistEnd::C_ARGLIST_END);
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif