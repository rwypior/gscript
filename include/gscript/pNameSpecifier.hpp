#ifndef _h_gscript_parser_namespecifier
#define _h_gscript_parser_namespecifier

#include "pEntity.hpp"
#include "EntityPath.hpp"

#include <string>

namespace gscript
{
	class ParserNameSpecifier : public ParserEntity
	{
	protected:
		static bool validateChar(std::string::const_iterator it, StringIteratorRange itrange);

	public:
		const char CHR_MEMBER_ACCESSOR = '.';

		EntityPath name;

		ParserNameSpecifier(const std::string &name = "");

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif