#ifndef _h_parser_vardeclaration
#define _h_parser_vardeclaration

#include "pEntity.hpp"
#include "pStatement.hpp"

namespace gscript
{
	class ParserVarDeclaration : public ParserEntity
	{
	public:
		std::string type;
		std::string name;
		bool sub;

		ParserStatement value;

		ParserVarDeclaration(bool sub = false);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif