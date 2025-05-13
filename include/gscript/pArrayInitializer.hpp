#ifndef _h_parser_array_initializer
#define _h_parser_array_initializer

#include "pEntity.hpp"
#include "pCallArglist.hpp"

namespace gscript
{
	class ParserArrayInitializer : public ParserEntity
	{
	public:
		static const char KW_ARRAY_INITIALIZER_BEGIN;
		static const char KW_ARRAY_INITIALIZER_END;

		ParserCallArglist arglist;

		ParserArrayInitializer();
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif
