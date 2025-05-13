#ifndef _h_typespecifier
#define _h_typespecifier

#include "pNameSpecifier.hpp"

namespace gscript
{
	class ParserTypeSpecifier : public ParserNameSpecifier
	{
	public:
		bool isReference = false;
		bool isArray = false;
		size_t arraySize;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif