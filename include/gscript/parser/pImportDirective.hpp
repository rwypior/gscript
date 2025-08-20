#ifndef _h_gscript_parser_control_import
#define _h_gscript_parser_control_import

#include "pEntity.hpp"
#include "pControlDirective.hpp"
#include "IteratorRange.hpp"

#include <string>

namespace gscript
{
	class ParserImportDirective : public ParserControlDirective
	{
	public:
		enum class IMPORT_TYPE_T
		{
			IT_INVALID = 0,
			IT_FILE = 1,
			IT_EXTENSION = 2
		};

		static const char *DIRECTIVE_IMPORT;

		static const char ENCLOSURE_BEGIN;
		static const char ENCLOSURE_END;

		static const char EXTENSION_ENCLOSURE_BEGIN;
		static const char EXTENSION_ENCLOSURE_END;

		//char enclosureBegin;
		//char enclosureEnd;

		IMPORT_TYPE_T type = IMPORT_TYPE_T::IT_INVALID;

		std::string filename;

		//ParserImportDirective(char enclosureBegin = ENCLOSURE_BEGIN, char enclosureEnd = ENCLOSURE_END);
		ParserImportDirective();

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif