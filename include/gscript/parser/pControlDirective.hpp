#ifndef _h_gscript_parser_control_directive
#define _h_gscript_parser_control_directive

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserControlDirective : public ParserEntity
	{
	public:
		static constexpr char keycharControl = '#';
		std::string directive;

		ParserControlDirective(const std::string &directive);
		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif