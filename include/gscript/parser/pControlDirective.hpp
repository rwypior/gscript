#ifndef _h_gscript_parser_control_directive
#define _h_gscript_parser_control_directive

#include "pEntity.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserControlDirective : public ParserEntity
	{
	public:
		static const char C_CONTROL;
		std::string directive;

		ParserControlDirective(const std::string &directive);
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif