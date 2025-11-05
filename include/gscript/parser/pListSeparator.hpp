#ifndef _h_gscript_parser_listseparator
#define _h_gscript_parser_listseparator

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserListSeparator : public ParserEntity
	{
	public:
		static constexpr char keycharDefaultSeparator = ',';

		const char separatorChar = keycharDefaultSeparator;

		ParserListSeparator(const char separator = keycharDefaultSeparator);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif