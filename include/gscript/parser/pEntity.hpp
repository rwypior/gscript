#ifndef _h_gscript_parser_entity
#define _h_gscript_parser_entity

#include "gscript/IteratorRange.hpp"
#include "gscript/parser/ParseResult.hpp"
#include "gscript/parser/pInfo.hpp"

#include <string>

namespace gscript
{
	/// A base class for all parser entities. Every parsing entity should inherit this class
	/// and provide specialized parsing for themselves.
	/// Usually the parsing starts with the namespace parser, which owns parsing of child entities.
	class ParserEntity
	{
	public:
		virtual ~ParserEntity() = default;

		/// Parse the entity
		/// @param itrange A string iterator range of source code to be parsed
		virtual ParseResult parse(StringIteratorRange itrange) = 0;

		ParserInfo info;
	};
}

#endif