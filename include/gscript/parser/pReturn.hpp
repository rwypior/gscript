#ifndef _h_gscript_parser_return
#define _h_gscript_parser_return

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserReturn : public ParserEntity
	{
	public:
		const std::string KW_RETURN = "return";

		ParserStatement value;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif