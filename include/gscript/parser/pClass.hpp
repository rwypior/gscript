#ifndef _h_gscript_parser_class
#define _h_gscript_parser_class

#include "pEntity.hpp"
#include "pMethod.hpp"
#include "pFieldDeclaration.hpp"
#include "IteratorRange.hpp"

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