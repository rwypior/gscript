#ifndef _h_gscript_parser_operator
#define _h_gscript_parser_operator

#include "pEntity.hpp"
//#include "operator.hpp"
#include "utilParserWord.hpp"
#include "defs.hpp"

#include <string>
#include <cassert>

//#define P_OP_CONVERT(P_OP) ScriptOperator *createOperator(ScriptScope &scope) const override { return new P_OP::SCRIPT_OPERATOR(scope, this->getLinkage()); }
//#define P_OP_PARSE(P_OP) ParseResult parse(StringIteratorRange itrange) { return Util::Word::parse(itrange, this->opchar, new P_OP(*this), true); }

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
		static const std::string KW_OPERATOR;

		ParserOperatorMemberAccessor()
			: ParserOperator(".")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorMemberAccessor>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MEMBER_ACCESSOR;
		}
	};

	class ParserOperatorNegate : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorNegate()
			: ParserOperator("!")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorNegate>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_NEGATE;
		}
	};

	class ParserOperatorAssign : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorAssign()
			: ParserOperator("=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorAssign>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ASSIGN;
		}
	};

	class ParserOperatorEquals : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorEquals()
			: ParserOperator("==")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorEquals>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_EQUALS;
		}
	};

	class ParserOperatorGreaterThan : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorGreaterThan()
			: ParserOperator(">")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorGreaterThan>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_GREATER_THAN;
		}
	};

	class ParserOperatorLessThan : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorLessThan()
			: ParserOperator("<")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorLessThan>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_LESSER_THAN;
		}
	};

	class ParserOperatorGreaterThanOrEqual : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorGreaterThanOrEqual()
			: ParserOperator(">=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorGreaterThanOrEqual>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL;
		}
	};

	class ParserOperatorLessThanOrEqual : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorLessThanOrEqual()
			: ParserOperator("<=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorLessThanOrEqual>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL;
		}
	};

	class ParserOperatorAdd : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorAdd()
			: ParserOperator("+")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorAdd>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ADD;
		}
	};

	class ParserOperatorAddTo : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorAddTo()
			: ParserOperator("+=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorAddTo>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_ADD_TO;
		}
	};

	class ParserOperatorSubtract : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorSubtract()
			: ParserOperator("-")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorSubtract>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_SUBTRACT;
		}
	};

	class ParserOperatorSubtractFrom : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorSubtractFrom()
			: ParserOperator("-=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorSubtractFrom>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_SUBTRACT_FROM;
		}
	};

	class ParserOperatorMultiply : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorMultiply()
			: ParserOperator("*")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorMultiply>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MULTIPLY;
		}
	};

	class ParserOperatorMultiplyBy : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorMultiplyBy()
			: ParserOperator("*=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorMultiplyBy>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_MULTIPLY_BY;
		}
	};

	class ParserOperatorDivide : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorDivide()
			: ParserOperator("/")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorDivide>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DIVIDE;
		}
	};

	class ParserOperatorDivideBy : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorDivideBy()
			: ParserOperator("/=")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorDivideBy>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DIVIDE_BY;
		}
	};

	class ParserOperatorIncrement : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorIncrement()
			: ParserOperator("++")
		{
		}

		ParserOperatorIncrement(OPERATOR_POSITION_T position)
			: ParserOperator("++", position)
		{
		}

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorIncrement>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_INCREMENT;
		}
	};

	class ParserOperatorDecrement : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorDecrement(OPERATOR_POSITION_T position)
			: ParserOperator("--", position)
		{
		}

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorDecrement>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_DECREMENT;
		}
	};

	class ParserOperatorConditionalNull : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorConditionalNull()
			: ParserOperator("?:")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalNull>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_NULL;
		}
	};

	class ParserOperatorConditionalA : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorConditionalA()
			: ParserOperator("?")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalA>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_IF;
		}
	};

	class ParserOperatorConditionalB : public ParserOperator
	{
	public:
		static const std::string KW_OPERATOR;

		ParserOperatorConditionalB()
			: ParserOperator(":")
		{
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return Util::Word::parse(itrange, this->opchar, std::make_shared<ParserOperatorConditionalB>(*this), true);
		}

		virtual OPERATOR_TYPE_T getType() const override
		{
			return OPERATOR_TYPE_T::OT_CONDITIONAL_ELSE;
		}
	};
}

#endif