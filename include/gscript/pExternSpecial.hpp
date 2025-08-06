#ifndef _h_gscript_parser_extern_special
#define _h_gscript_parser_extern_special

#include "pEntity.hpp"
#include "pSpecialDirective.hpp"

#include <string>

namespace gscript
{
	class ParserExternSpecial : public ParserSpecialDirective
	{
	public:
		static const char *DIRECTIVE_EXTERN;

		char terminator = 0;

		std::string name;

		ParserExternSpecial(char terminator = ParserSpecialDirective::C_CONTROL_PARAM_TERMINATOR);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif