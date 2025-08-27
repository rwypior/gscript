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
		static const char *KW_M_AS_PUBLIC;
		static const char *KW_M_AS_PRIVATE;
		static const char *KW_M_AS_PROTECTED;

		static const char *KW_M_CONST;

		static const char *KW_M_STATIC;

		static const char *KW_M_VIRTUAL;

		//std::string access;
		MODIFIER_T modifier = MODIFIER_T::M_NONE;

		virtual ParseResult parse(StringIteratorRange itrange) override;

		MODIFIER_T getModifier() const;

		//operator MODIFIER_T() const;
		//ParserAccessSpecifier &operator =(const ParserAccessSpecifier &specifier);
	};
}

#endif