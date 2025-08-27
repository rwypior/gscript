#ifndef _h_gscript_parser_array_accessor
#define _h_gscript_parser_array_accessor

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"
#include "bitfield.hpp"

namespace gscript
{
	class ParserArrayAccessor : public ParserEntity
	{
	public:
		enum class IndexType
		{
			None = 0,
			Required = 1,
			Optional = 2,
			Literal = 4,
			Statement = 8
		};

	public:
		static const char KW_ARRAY_ACCESSOR_BEGIN;
		static const char KW_ARRAY_ACCESSOR_END;

		ParserArrayAccessor(IndexType indexType);

		bool gotValue = false;
		size_t staticIndex = 0;
		ParserStatement statement;

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		IndexType indexType = IndexType::None;
	};
}

DEFINE_BITFIELD(gscript::ParserArrayAccessor::IndexType);

#endif
