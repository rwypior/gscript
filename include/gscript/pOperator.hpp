#ifndef _h_parser_operator
#define _h_parser_operator

#include "pEntity.hpp"
#include "operator.hpp"
#include "utilParserWord.hpp"

#include <string>
#include <cassert>

#define P_OP_CONVERT(P_OP) ScriptOperator *createOperator(ScriptScope &scope) const override { return new P_OP::SCRIPT_OPERATOR(scope, this->getLinkage()); }
#define P_OP_PARSE(P_OP) ParseResult parse(StringIteratorRange itrange) { return Util::Word::parse(itrange, this->opchar, new P_OP(*this), true); }

namespace gscript
{
	class ParserOperator : public ParserEntity
	{
	protected:
		OPERATOR_POSITION_T position;
		std::string opchar;

	public:
		ParserOperator(OPERATOR_POSITION_T position);
		ParserOperator(const char *op = "", OPERATOR_POSITION_T position = OPERATOR_POSITION_T::OP_IRRELEVANT);
		//ParserOperator(const char *op = "");

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

		// TODO this should be refactored
		virtual ScriptOperator *createOperator(ScriptScope &scope) const
		{
			assert(false && "Invalid operator");
			return NULL;
		}
	};

	class ParserOperatorMemberAccessor : public ParserOperator
	{
	public:
		typedef ScriptOperatorMemberAccessor SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorMemberAccessor()
			: ParserOperator(".")
		{
		}

		P_OP_CONVERT(ParserOperatorMemberAccessor);
		P_OP_PARSE(ParserOperatorMemberAccessor);
	};

	class ParserOperatorNegate : public ParserOperator
	{
	public:
		typedef ScriptOperatorNegate SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorNegate()
			: ParserOperator("!")
		{
		}

		P_OP_CONVERT(ParserOperatorNegate);
		P_OP_PARSE(ParserOperatorNegate);
	};

	class ParserOperatorAssign : public ParserOperator
	{
	public:
		typedef ScriptOperatorAssign SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorAssign()
			: ParserOperator("=")
		{
		}

		P_OP_CONVERT(ParserOperatorAssign);
		P_OP_PARSE(ParserOperatorAssign);
	};

	class ParserOperatorEquals : public ParserOperator
	{
	public:
		typedef ScriptOperatorEquals SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorEquals()
			:ParserOperator("==")
		{ }

		P_OP_CONVERT(ParserOperatorEquals);
		P_OP_PARSE(ParserOperatorEquals);
	};

	class ParserOperatorGreaterThan : public ParserOperator
	{
	public:
		typedef ScriptOperatorGreaterThan SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorGreaterThan()
			:ParserOperator(">")
		{ }

		P_OP_CONVERT(ParserOperatorGreaterThan);
		P_OP_PARSE(ParserOperatorGreaterThan);
	};

	class ParserOperatorLessThan : public ParserOperator
	{
	public:
		typedef ScriptOperatorLessThan SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorLessThan()
			:ParserOperator("<")
		{ }

		P_OP_CONVERT(ParserOperatorLessThan);
		P_OP_PARSE(ParserOperatorLessThan);
	};

	class ParserOperatorGreaterThanOrEqual : public ParserOperator
	{
	public:
		typedef ScriptOperatorGreaterThanOrEqual SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorGreaterThanOrEqual()
			:ParserOperator(">=")
		{ }

		P_OP_CONVERT(ParserOperatorGreaterThanOrEqual);
		P_OP_PARSE(ParserOperatorGreaterThanOrEqual);
	};

	class ParserOperatorLessThanOrEqual : public ParserOperator
	{
	public:
		typedef ScriptOperatorLessThanOrEqual SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorLessThanOrEqual()
			:ParserOperator("<=")
		{ }

		P_OP_CONVERT(ParserOperatorLessThanOrEqual);
		P_OP_PARSE(ParserOperatorLessThanOrEqual);
	};

	class ParserOperatorAdd : public ParserOperator
	{
	public:
		typedef ScriptOperatorAdd SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorAdd()
			:ParserOperator("+")
		{ }

		P_OP_CONVERT(ParserOperatorAdd);
		P_OP_PARSE(ParserOperatorAdd);
	};

	class ParserOperatorAddTo : public ParserOperator
	{
	public:
		typedef ScriptOperatorAddTo SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorAddTo()
			:ParserOperator("+=")
		{ }

		P_OP_CONVERT(ParserOperatorAddTo);
		P_OP_PARSE(ParserOperatorAddTo);
	};

	class ParserOperatorSubtract : public ParserOperator
	{
	public:
		typedef ScriptOperatorSubtract SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorSubtract()
			:ParserOperator("-")
		{ }

		P_OP_CONVERT(ParserOperatorSubtract);
		P_OP_PARSE(ParserOperatorSubtract);
	};

	class ParserOperatorSubtractFrom : public ParserOperator
	{
	public:
		typedef ScriptOperatorSubtractFrom SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorSubtractFrom()
			:ParserOperator("-=")
		{ }

		P_OP_CONVERT(ParserOperatorSubtractFrom);
		P_OP_PARSE(ParserOperatorSubtractFrom);
	};

	class ParserOperatorMultiply : public ParserOperator
	{
	public:
		typedef ScriptOperatorMultiply SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorMultiply()
			:ParserOperator("*")
		{ }

		P_OP_CONVERT(ParserOperatorMultiply);
		P_OP_PARSE(ParserOperatorMultiply);
	};

	class ParserOperatorMultiplyBy : public ParserOperator
	{
	public:
		typedef ScriptOperatorMultiplyBy SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorMultiplyBy()
			:ParserOperator("*=")
		{ }

		P_OP_CONVERT(ParserOperatorMultiplyBy);
		P_OP_PARSE(ParserOperatorMultiplyBy);
	};

	class ParserOperatorDivide : public ParserOperator
	{
	public:
		typedef ScriptOperatorDivide SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorDivide()
			:ParserOperator("/")
		{ }

		P_OP_CONVERT(ParserOperatorDivide);
		P_OP_PARSE(ParserOperatorDivide);
	};

	class ParserOperatorDivideBy : public ParserOperator
	{
	public:
		typedef ScriptOperatorDivideBy SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorDivideBy()
			:ParserOperator("/=")
		{ }

		P_OP_CONVERT(ParserOperatorDivideBy);
		P_OP_PARSE(ParserOperatorDivideBy);
	};

	class ParserOperatorIncrement : public ParserOperator
	{
	public:
		typedef ScriptOperatorIncrement SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorIncrement()
			:ParserOperator("++")
		{ }

		ParserOperatorIncrement(OPERATOR_POSITION_T position)
			:ParserOperator("++", position)
		{ }

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		P_OP_CONVERT(ParserOperatorIncrement);
		P_OP_PARSE(ParserOperatorIncrement);
	};

	class ParserOperatorDecrement : public ParserOperator
	{
	public:
		typedef ScriptOperatorDecrement SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorDecrement(OPERATOR_POSITION_T position)
			:ParserOperator("--", position)
		{ }

		virtual OPERATOR_LINK_T getLinkage() const override
		{
			return this->position == OPERATOR_POSITION_T::OP_LEFT ? OPERATOR_LINK_T::OL_LEFT : OPERATOR_LINK_T::OL_RIGHT;
		}

		P_OP_CONVERT(ParserOperatorDecrement);
		P_OP_PARSE(ParserOperatorDecrement);
	};

	class ParserOperatorConditionalNull : public ParserOperator
	{
	public:
		typedef ScriptOperatorConditionalNull SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorConditionalNull()
			:ParserOperator("?:")
		{ }

		P_OP_CONVERT(ParserOperatorConditionalNull);
		P_OP_PARSE(ParserOperatorConditionalNull);
	};

	class ParserOperatorConditionalA : public ParserOperator
	{
	public:
		typedef ScriptOperatorConditionalA SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorConditionalA()
			:ParserOperator("?")
		{ }

		P_OP_CONVERT(ParserOperatorConditionalA);
		P_OP_PARSE(ParserOperatorConditionalA);
	};

	class ParserOperatorConditionalB : public ParserOperator
	{
	public:
		typedef ScriptOperatorConditionalB SCRIPT_OPERATOR;

		static const std::string KW_OPERATOR;

		ParserOperatorConditionalB()
			:ParserOperator(":")
		{ }

		P_OP_CONVERT(ParserOperatorConditionalB);
		P_OP_PARSE(ParserOperatorConditionalB);
	};
}

#endif