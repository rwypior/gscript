#ifndef _h_gscript_parser_callarglist
#define _h_gscript_parser_callarglist

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pListSeparator.hpp"
#include "gscript/IteratorRange.hpp"
#include "gscript/defs.hpp"

#include <vector>

namespace gscript
{
	class ParserStatement;

	class ParserCallArglist : public ParserEntity
	{
	public:
		int maxCount = 0;
		int minCount = 0;
		char separator = 0;
		char start = 0;
		char end = 0;

		std::vector<std::unique_ptr<ParserStatement>> parameters;

		ParserCallArglist(
			const int maxCount = 0,
			const int minCount = 0,
			const char separator = ',',
			char start = ParserArglistStart::keycharArglistStart,
			char end = ParserArglistEnd::keycharArglistEnd
		);
		ParserCallArglist(const ParserCallArglist&) = delete;
		ParserCallArglist(ParserCallArglist&& b) noexcept
			: parameters(std::move(b.parameters))
		{
		}

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif