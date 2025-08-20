#ifndef _h_gscript_parser_new
#define _h_gscript_parser_new

#include "pEntity.hpp"
#include "pFuncCall.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserNew : public ParserFuncCall
	{
	public:
		const char *KW_NEW = "new";

		std::string constructorName;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif