#ifndef _h_gscript_parser_function
#define _h_gscript_parser_function

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pArglist.hpp"
#include "gscript/parser/pBlockBody.hpp"
#include "gscript/defs.hpp"
#include "gscript/type.hpp"
#include "gscript/IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserFunction : public ParserEntity
	{
	public:
		std::string name;
		std::string returnTypeName;

		std::string externName = "";

		TypeDescriptor returnTypeClassifier;
		ParserBlockBody body;

		ParserArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif