#ifndef _h_gscript_parser_array_accessor
#define _h_gscript_parser_array_accessor

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserArrayAccessor : public ParserEntity
	{
	public:
		enum INDEX_TYPE_T
		{
			IT_NONE = 0,
			IT_REQUIRED = 1,
			IT_OPTIONAL = 2,
			IT_LITERAL = 4,
			IT_STATEMENT = 8
		};

	public:
		static const char KW_ARRAY_ACCESSOR_BEGIN;
		static const char KW_ARRAY_ACCESSOR_END;

		ParserArrayAccessor(int indexType);

		bool gotValue = false;
		size_t staticIndex = 0;
		ParserStatement statement;

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		int indexType = 0;
	};
}

#endif
