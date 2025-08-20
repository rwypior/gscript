#ifndef _h_gscript_parser_begin
#define _h_gscript_parser_begin

#include "pEntity.hpp"
#include "pClass.hpp"
#include "pFunction.hpp"
#include "IteratorRange.hpp"

#include <vector>
#include <list>
#include <deque>
#include <string>

namespace gscript
{
	class ParserNamespace : public ParserEntity
	{
	public:
		static const char *KW_NAMESPACE;
		static const char KW_ENCLOSURE_BEGIN;
		static const char KW_ENCLOSURE_END;

		typedef std::list<ParserNamespace> NAMESPACE_CONTAINER_T;
		typedef std::list<ParserClass> CLASS_CONTAINER_T;
		typedef std::list<ParserFunction> FUNCTION_CONTAINER_T;
		typedef std::deque<ParserEntity*> ENTITY_CONTAINER_T;
		typedef std::vector<std::string> IMPORT_CONTAINER_T;

		NAMESPACE_TYPE_T type;
		std::string name;

		NAMESPACE_CONTAINER_T namespaces;
		CLASS_CONTAINER_T classes;
		FUNCTION_CONTAINER_T functions;
		IMPORT_CONTAINER_T imports;
		IMPORT_CONTAINER_T extensions;

		ParserNamespace(NAMESPACE_TYPE_T type = NAMESPACE_TYPE_T::NT_REGULAR);

		virtual ParseResult parse(StringIteratorRange itrange) override;

		bool findClass(const std::string &name, ParserClass &pClass);
	};
}

#endif