#ifndef _h_parser_function
#define _h_parser_function

#include "pEntity.hpp"
#include "pArglist.hpp"
#include "pBlockBody.hpp"
#include "defs.hpp"
#include "type.hpp"

#include <string>
#include <memory>

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