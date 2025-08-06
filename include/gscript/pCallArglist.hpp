#ifndef _h_gscript_parser_callarglist
#define _h_gscript_parser_callarglist

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

	class ParserCallArglist : public ParserEntity
	{
	public:
		typedef std::vector<ParserStatement*> STATEMENT_CONTAINER_T;

		int maxCount = 0;
		int minCount = 0;
		char separator = 0;
		char start = 0;
		char end = 0;

		BITFLAG_T allowedEntities = 0x00;

		STATEMENT_CONTAINER_T parameters;

		ParserCallArglist(
			const int maxCount = 0,
			const int minCount = 0,
			const char separator = ',',
			char start = ParserArglistStart::C_ARGLIST_START,
			char end = ParserArglistEnd::C_ARGLIST_END,
			BITFLAG_T allowedEntities = static_cast<int>(ENTITY_TYPE_T::ET_STATEMENT)
		);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif