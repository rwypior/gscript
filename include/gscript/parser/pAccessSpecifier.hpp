#ifndef _h_gscript_parser_accessspecifier
#define _h_gscript_parser_accessspecifier

#include "gscript/parser/pEntity.hpp"
#include "gscript/defs.hpp"
#include "gscript/IteratorRange.hpp"
#include "gscript/bitfield.hpp"

namespace gscript
{
	class ParserAccessSpecifier : public ParserEntity
	{
	public:
		static constexpr char keywordPublic[] = "public";
		static constexpr char keywordPrivate[] = "private";
		static constexpr char keywordProtected[] = "protected";
		static constexpr char keywordConst[] = "const";
		static constexpr char keywordStatic[] = "static";
		static constexpr char keywordVirtual[] = "virtual";

		Modifier modifier = Modifier::None;

		ParserAccessSpecifier(Modifier defaultModifier = Modifier::None);
		virtual ParseResult parse(StringIteratorRange itrange) override;

		Modifier getModifier() const;
	};
}

#endif