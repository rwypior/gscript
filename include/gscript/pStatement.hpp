#ifndef _h_gscript_parser_statement
#define _h_gscript_parser_statement

#include "pEntity.hpp"

#include <vector>
#include <memory>

namespace gscript
{
	class ParserStatement : public ParserEntity
	{
	public:
		typedef std::vector<std::shared_ptr<ParserEntity>> CONTAINER_T;

	public:
		CONTAINER_T components;

		ParserStatement(bool isSubStatement = false, bool forceEnclosing = false);
		~ParserStatement();

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		bool isSubStatement = false;
		bool forceEnclosing = false;
	};
}

#endif