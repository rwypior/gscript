#ifndef _h_gscript_operator_declaration
#define _h_gscript_operator_declaration

#include "gscript/defs.hpp"
#include "gscript/IteratorRange.hpp"
#include "gscript/parser/pEntity.hpp"

#include <map>
#include <string>

namespace gscript
{
	class ParserOperatorDeclaration : public ParserEntity
	{
	public:
		static constexpr char keywordOperator[] = "operator";

		static constexpr char keywordOperatorAdd[] = "+";
		static constexpr char keywordOperatorTo[] = "+=";
		static constexpr char keywordSubtract[] = "-";
		static constexpr char keywordSubtractFrom[] = "-=";
		static constexpr char keywordMultiply[] = "*";
		static constexpr char keywordMultiplyBy[] = "*=";
		static constexpr char keywordDivide[] = "/";
		static constexpr char keywordDivideBy[] = "/=";
		static constexpr char keywordEquals[] = "==";
		static constexpr char keywordNotEquals[] = "!=";
		static constexpr char keywordGreaterThan[] = ">";
		static constexpr char keywordGreaterThanOrEqual[] = ">=";
		static constexpr char keywordLessThan[] = "<";
		static constexpr char keywordLessThanOrEqual[] = "<=";
		static constexpr char keywordAssign[] = "=";
		static constexpr char keywordNegate[] = "!";
		static constexpr char keywordPostIncrement[] = ".++";
		static constexpr char keywordPreIncrement[] = "++.";
		static constexpr char keywordPostDecrement[] = ".--";
		static constexpr char keywordPreDecrement[] = "--.";
		static constexpr char keywordCall[] = "()";

		OperatorType operatorType;

		virtual ParseResult parse(StringIteratorRange itrange) override;

		static OperatorType translateOperator(const std::string& oper);
	};
}

#endif