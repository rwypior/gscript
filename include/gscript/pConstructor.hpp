#ifndef _h_gscript_parser_constructor
#define _h_gscript_parser_constructor

#include "pEntity.hpp"
#include "pMethod.hpp"
#include "pAccessSpecifier.hpp"

#include <vector>

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