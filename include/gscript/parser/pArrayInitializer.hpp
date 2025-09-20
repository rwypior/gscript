#ifndef _h_gscript_parser_array_initializer
#define _h_gscript_parser_array_initializer

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "IteratorRange.hpp"

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
