#ifndef _h_gscript_parser_array_initializer
#define _h_gscript_parser_array_initializer

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserArrayInitializer : public ParserEntity
	{
	public:
		static constexpr char keycharArrayInitializerBegin = '{';
		static constexpr char keycharArrayInitializerEnd = '}';

		ParserCallArglist arglist;

		ParserArrayInitializer();
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif
