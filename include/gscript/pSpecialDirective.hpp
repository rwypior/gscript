#ifndef _h_gscript_parser_special_directive
#define _h_gscript_parser_special_directive

#include "pEntity.hpp"

#include <string>

namespace gscript
{
	class ParserSpecialDirective : public ParserEntity
	{
	public:
		static const char C_CONTROL;
		static const char C_CONTROL_PARAM;
		static const char C_CONTROL_PARAM_TERMINATOR;

		std::string directive;
		bool parameterized = false;

		ParserSpecialDirective(const std::string &directive, bool parameterized = false);
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif