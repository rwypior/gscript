#ifndef _h_gscript_parser_forarglist
#define _h_gscript_parser_forarglist

#include "pEntity.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pVarDeclaration.hpp"
#include "pListSeparator.hpp"

#include "defs.hpp"

#include <vector>

namespace gscript
{
	class ParserStatement;

	class ParserForArglist : public ParserEntity
	{
	public:
		char separator;
		char start;
		char end;

		ParserVarDeclaration *varDecl = nullptr;
		ParserStatement *condition = nullptr;
		ParserStatement *progress = nullptr;

		ParserForArglist(
			const char separator = ';',
			char start = ParserArglistStart::C_ARGLIST_START,
			char end = ParserArglistEnd::C_ARGLIST_END
		);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif