#ifndef _h_gscript_typespecifier
#define _h_gscript_typespecifier

#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserTypeSpecifier : public ParserNameSpecifier
	{
	public:
		bool isReference = false;
		bool isArray = false;
		size_t arraySize = 0;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif