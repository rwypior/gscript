#ifndef _h_gscript_parser_extern_special
#define _h_gscript_parser_extern_special

#include "pEntity.hpp"
#include "pSpecialDirective.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserExternSpecial : public ParserSpecialDirective
	{
	public:
		static constexpr char keywordExtern[] = "extern";

		const char terminator = ParserSpecialDirective::keycharControlParamTerminator;

		std::string name;

		ParserExternSpecial(char terminator = ParserSpecialDirective::keycharControlParamTerminator);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif