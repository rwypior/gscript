#ifndef _h_gscript_parser_return
#define _h_gscript_parser_return

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserReturn : public ParserEntity
	{
	public:
		static constexpr char keywordReturn[] = "return";

		ParserStatement value;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif