#ifndef _h_gscript_parser_entity
#define _h_gscript_parser_entity

#include <vector>
#include <string>

#include "IteratorRange.hpp"
#include "ParseResult.hpp"

namespace gscript
{
	class ParserEntity
	{
	public:
		typedef IteratorRange<std::string> StringIteratorRange;

		virtual ParseResult parse(StringIteratorRange itrange) = 0;

		virtual std::string _name() const
		{
			return "unknown";
		}
	};
}

#endif