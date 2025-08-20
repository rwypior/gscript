#ifndef _h_gscript_parser_vardeclaration
#define _h_gscript_parser_vardeclaration

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserVarDeclaration : public ParserEntity
	{
	public:
		std::string type;
		std::string name;
		bool sub = false;

		ParserStatement value;

		ParserVarDeclaration(bool sub = false);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif