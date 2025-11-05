#ifndef _h_gscript_parser_begin
#define _h_gscript_parser_begin

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pClass.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/IteratorRange.hpp"

#include <vector>
#include <list>
#include <deque>
#include <string>

namespace gscript
{
	class ParserNamespace : public ParserEntity
	{
	public:
		static constexpr char keywordNamespace[] = "namespace";
		static constexpr char keycharEnclosureBegin = '{';
		static constexpr char keycharEnclosureEnd = '}';

		typedef std::list<ParserNamespace> NAMESPACE_CONTAINER_T;
		typedef std::list<ParserClass> CLASS_CONTAINER_T;
		typedef std::list<ParserFunction> FUNCTION_CONTAINER_T;
		typedef std::deque<ParserEntity*> ENTITY_CONTAINER_T;
		typedef std::vector<std::string> IMPORT_CONTAINER_T;

		NamespaceType type;
		std::string name;

		NAMESPACE_CONTAINER_T namespaces;
		CLASS_CONTAINER_T classes;
		FUNCTION_CONTAINER_T functions;
		IMPORT_CONTAINER_T imports;
		IMPORT_CONTAINER_T extensions;

		ParserNamespace(NamespaceType type = NamespaceType::Regular);

		virtual ParseResult parse(StringIteratorRange itrange) override;

		bool findClass(const std::string &name, ParserClass &pClass);
	};
}

#endif