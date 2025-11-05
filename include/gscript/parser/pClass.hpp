#ifndef _h_gscript_parser_class
#define _h_gscript_parser_class

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pMethod.hpp"
#include "gscript/parser/pFieldDeclaration.hpp"
#include "gscript/IteratorRange.hpp"

#include <vector>
#include <string>

namespace gscript
{
	class ParserClass : public ParserEntity
	{
	public:
		static constexpr char keywordClass[] = "class";

		std::vector<ParserMethod> methods;
		std::vector<ParserFieldDeclaration> fields;

		std::string name;
		std::string base;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif