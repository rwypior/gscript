#ifndef _h_gscript_parser_forarglist
#define _h_gscript_parser_forarglist

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/IteratorRange.hpp"
#include "gscript/defs.hpp"

namespace gscript
{
	class ParserStatement;

	class ParserForArglist : public ParserEntity
	{
	public:
		static constexpr char keycharSeparator = ';';

		char separator = keycharSeparator;
		char start = ParserArglistStart::keycharArglistStart;
		char end = ParserArglistEnd::keycharArglistEnd;

		ParserVarDeclaration *varDecl = nullptr;
		ParserStatement *condition = nullptr;
		ParserStatement *progress = nullptr;

		ParserForArglist(
			const char separator = keycharSeparator,
			char start = ParserArglistStart::keycharArglistStart,
			char end = ParserArglistEnd::keycharArglistEnd
		);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif