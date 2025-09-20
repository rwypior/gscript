#ifndef _h_gscript_parser_reference
#define _h_gscript_parser_reference

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserReference : public ParserEntity
	{
	public:
		static constexpr char keycharReference = '&';

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif
