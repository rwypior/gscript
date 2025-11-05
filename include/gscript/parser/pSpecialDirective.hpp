#ifndef _h_gscript_parser_special_directive
#define _h_gscript_parser_special_directive

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserSpecialDirective : public ParserEntity
	{
	public:
		static constexpr char keycharControl = '@';
		static constexpr char keycharControlParam = ':';
		static constexpr char keycharControlParamTerminator = '\n';

		std::string directive;
		bool parameterized = false;

		ParserSpecialDirective(const std::string &directive, bool parameterized = false);
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif