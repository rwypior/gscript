#ifndef _h_gscript_typespecifier
#define _h_gscript_typespecifier

#include "pNameSpecifier.hpp"
#include "IteratorRange.hpp"

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