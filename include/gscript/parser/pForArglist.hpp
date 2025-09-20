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