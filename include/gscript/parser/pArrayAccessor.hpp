#ifndef _h_gscript_parser_array_accessor
#define _h_gscript_parser_array_accessor

#include "gscript/parser/pEntity.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/IteratorRange.hpp"
#include "gscript/bitfield.hpp"

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
		static constexpr char keycharArrayAccessorBegin = '[';
		static constexpr char keycharArrayAccessorEnd = ']';

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
