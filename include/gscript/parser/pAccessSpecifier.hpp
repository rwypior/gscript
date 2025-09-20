#ifndef _h_gscript_parser_accessspecifier
#define _h_gscript_parser_accessspecifier

#include "pEntity.hpp"
#include "defs.hpp"
#include "IteratorRange.hpp"
#include "bitfield.hpp"

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

		MODIFIER_T modifier = MODIFIER_T::M_NONE;

		ParserAccessSpecifier(MODIFIER_T defaultModifier = MODIFIER_T::M_NONE);
		virtual ParseResult parse(StringIteratorRange itrange) override;

		MODIFIER_T getModifier() const;
	};
}

#endif