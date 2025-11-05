#ifndef _h_gscript_parser_statement
#define _h_gscript_parser_statement

#include "gscript/parser/pEntity.hpp"
#include "gscript/IteratorRange.hpp"

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

		ParserStatement(bool isSubStatement = false, bool forceEnclosing = false, bool allowEmpty = true);

		virtual ParseResult parse(StringIteratorRange itrange) override;

		void setAllowEmpty(bool allowEmpty = true);

	protected:
		bool isSubStatement = false;
		bool forceEnclosing = false;
		bool allowEmpty = true;
	};
}

#endif