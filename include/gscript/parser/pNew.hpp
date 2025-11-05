#ifndef _h_gscript_parser_new
#define _h_gscript_parser_new

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserNew : public ParserFuncCall
	{
	public:
		static constexpr char keywordNew[] = "new";

		std::string constructorName;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif