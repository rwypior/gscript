#ifndef _h_gscript_parser_forarglist
#define _h_gscript_parser_forarglist

#include "pEntity.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pVarDeclaration.hpp"
#include "pListSeparator.hpp"
#include "IteratorRange.hpp"
#include "defs.hpp"

namespace gscript
{
	class ParserStatement;

	class ParserForArglist : public ParserEntity
	{
	public:
		char separator = 0;
		char start = 0;
		char end = 0;

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