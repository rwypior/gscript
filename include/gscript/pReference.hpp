#ifndef _h_parser_reference
#define _h_parser_reference

#include "pEntity.hpp"
#include "pStatement.hpp"

namespace gscript
{
	class ParserReference : public ParserEntity
	{
	public:
		static const char KW_REFERENCE;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif
