#ifndef _h_gscript_parser_entity
#define _h_gscript_parser_entity

#include "IteratorRange.hpp"
#include "ParseResult.hpp"

#include <string>

namespace gscript
{
	class ParserEntity
	{
	public:
		virtual ParseResult parse(StringIteratorRange itrange) = 0;

		virtual std::string _name() const
		{
			return "unknown";
		}
	};
}

#endif