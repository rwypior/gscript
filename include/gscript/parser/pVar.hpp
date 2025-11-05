#ifndef _h_gscript_parser_var
#define _h_gscript_parser_var

#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/IteratorRange.hpp"

#include <memory>

namespace gscript
{
	class ParserVar : public ParserNameSpecifier
	{
	public:
		std::unique_ptr<ParserArrayAccessor> arrayAccessor = nullptr;

		ParserVar() = default;
		ParserVar(const ParserVar &copy);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif