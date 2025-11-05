#ifndef _h_gscript_parser_namespecifier
#define _h_gscript_parser_namespecifier

#include "gscript/parser/pEntity.hpp"
#include "gscript/EntityPath.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserNameSpecifier : public ParserEntity
	{
	public:
		EntityPath name;

		ParserNameSpecifier(const std::string &name = "");

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		static bool validateChar(std::string::const_iterator it, StringIteratorRange itrange);
	};
}

#endif