#ifndef _h_parser_var
#define _h_parser_var

#include "pNameSpecifier.hpp"

namespace gscript
{
	class ParserArrayAccessor;

	class ParserVar : public ParserNameSpecifier
	{
	public:
		ParserArrayAccessor * arrayAccessor = NULL;

		ParserVar() = default;
		ParserVar(const ParserVar &copy);
		~ParserVar();

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif