#ifndef _h_gscript_parser_operator
#define _h_gscript_parser_operator

#include "pEntity.hpp"
#include "parser/pWord.hpp"
#include "defs.hpp"

#include <string>
#include <cassert>

namespace gscript
{
	class ParserOperator : public ParserEntity
	{
	public:
		ParserOperator(OPERATOR_POSITION_T position);
		ParserOperator(const char *op = "", OPERATOR_POSITION_T position = OPERATOR_POSITION_T::OP_IRRELEVANT);

		virtual ParseResult parse(StringIteratorRange itrange) override;

		const std::string &getChar() const;

		virtual std::string _name()
		{
			return this->getChar();
		}

		virtual OPERATOR_LINK_T getLinkage() const
		{
			return OPERATOR_LINK_T::OL_BOTH;
		}

		virtual OPERATOR_TYPE_T getType() const
		{
			return OPERATOR_TYPE_T::OT_INVALID;
		}

	protected:
		OPERATOR_POSITION_T position;
		std::string opchar;
	};

	class ParserOperatorMemberAccessor : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = ".";

		ParserOperatorMemberAccessor()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorMemberAccessor>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MEMBER_ACCESSOR;
		}
	};

	class ParserOperatorNegate : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "!";

		ParserOperatorNegate()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorNegate>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_NEGATE;
		}
	};

	class ParserOperatorAssign : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "=";

		ParserOperatorAssign()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorAssign>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ASSIGN;
		}
	};

	class ParserOperatorEquals : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "==";

		ParserOperatorEquals()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorEquals>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_EQUALS;
		}
	};

	class ParserOperatorGreaterThan : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = ">";

		ParserOperatorGreaterThan()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorGreaterThan>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_GREATER_THAN;
		}
	};

	class ParserOperatorLessThan : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "<";

		ParserOperatorLessThan()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorLessThan>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_LESSER_THAN;
		}
	};

	class ParserOperatorGreaterThanOrEqual : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = ">=";

		ParserOperatorGreaterThanOrEqual()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorGreaterThanOrEqual>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL;
		}
	};

	class ParserOperatorLessThanOrEqual : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "<=";

		ParserOperatorLessThanOrEqual()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorLessThanOrEqual>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL;
		}
	};

	class ParserOperatorAdd : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "+";

		ParserOperatorAdd()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorAdd>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ADD;
		}
	};

	class ParserOperatorAddTo : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "+=";

		ParserOperatorAddTo()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorAddTo>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ADD_TO;
		}
	};

	class ParserOperatorSubtract : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "-";

		ParserOperatorSubtract()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorSubtract>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_SUBTRACT;
		}
	};

	class ParserOperatorSubtractFrom : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "-=";

		ParserOperatorSubtractFrom()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorSubtractFrom>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_SUBTRACT_FROM;
		}
	};

	class ParserOperatorMultiply : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "*";

		ParserOperatorMultiply()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorMultiply>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MULTIPLY;
		}
	};

	class ParserOperatorMultiplyBy : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "*=";

		ParserOperatorMultiplyBy()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorMultiplyBy>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MULTIPLY_BY;
		}
	};

	class ParserOperatorDivide : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "/";

		ParserOperatorDivide()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorDivide>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DIVIDE;
		}
	};

	class ParserOperatorDivideBy : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "/=";

		ParserOperatorDivideBy()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorDivideBy>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DIVIDE_BY;
		}
	};

	class ParserOperatorIncrement : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "++";

		ParserOperatorIncrement()
			: ParserOperator(keywordOperator)
		{
		}

		ParserOperatorIncrement(OPERATOR_POSITION_T position)
			: ParserOperator(keywordOperator, position)
		{
		}

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorIncrement>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_INCREMENT;
		}
	};

	class ParserOperatorDecrement : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "--";

		ParserOperatorDecrement(OPERATOR_POSITION_T position)
			: ParserOperator(keywordOperator, position)
		{
		}

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorDecrement>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DECREMENT;
		}
	};

	class ParserOperatorConditionalNull : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "?:";

		ParserOperatorConditionalNull()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalNull>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_NULL;
		}
	};

	class ParserOperatorConditionalA : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "?";

		ParserOperatorConditionalA()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalA>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_IF;
		}
	};

	class ParserOperatorConditionalB : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = ":";

		ParserOperatorConditionalB()
			: ParserOperator(keywordOperator)
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalB>(*this)/*, true*/);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_ELSE;
		}
	};
}

#endif