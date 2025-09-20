#ifndef _h_gscript_parser_endstatement
#define _h_gscript_parser_endstatement

#include "pEntity.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserEndStatement : public ParserEntity
	{
	public:
		static constexpr char keycharEndStatement = ';';

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif