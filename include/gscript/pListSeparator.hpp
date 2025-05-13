#ifndef _h_parser_listseparator
#define _h_parser_listseparator

#include "pEntity.hpp"

namespace gscript
{
	class ParserListSeparator : public ParserEntity
	{
	public:
		const char KW_SEPARATOR = ',';

		ParserListSeparator(const char separator = ',');

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif