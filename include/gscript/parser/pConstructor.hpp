#ifndef _h_gscript_parser_constructor
#define _h_gscript_parser_constructor

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pMethod.hpp"
#include "gscript/parser/pAccessSpecifier.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserClass;

	class ParserConstructor : public ParserMethod
	{
	public:
		ParserConstructor(ParserClass &pClass);

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		ParserClass &pClass;
	};
}

#endif