#include "parser/pOperator.hpp"
#include "parser/pWord.hpp"

namespace gscript
{
	const std::string ParserOperatorMemberAccessor::KW_OPERATOR = ".";
	const std::string ParserOperatorNegate::KW_OPERATOR = "!";
	const std::string ParserOperatorAssign::KW_OPERATOR = "=";
	const std::string ParserOperatorEquals::KW_OPERATOR = "==";
	const std::string ParserOperatorGreaterThan::KW_OPERATOR = ">";
	const std::string ParserOperatorLessThan::KW_OPERATOR = "<";
	const std::string ParserOperatorGreaterThanOrEqual::KW_OPERATOR = ">=";
	const std::string ParserOperatorLessThanOrEqual::KW_OPERATOR = "<=";
	const std::string ParserOperatorAdd::KW_OPERATOR = "+";
	const std::string ParserOperatorAddTo::KW_OPERATOR = "+=";
	const std::string ParserOperatorSubtract::KW_OPERATOR = "-";
	const std::string ParserOperatorSubtractFrom::KW_OPERATOR = "-=";
	const std::string ParserOperatorMultiply::KW_OPERATOR = "*";
	const std::string ParserOperatorMultiplyBy::KW_OPERATOR = "*=";
	const std::string ParserOperatorDivide::KW_OPERATOR = "/";
	const std::string ParserOperatorDivideBy::KW_OPERATOR = "/=";
	const std::string ParserOperatorIncrement::KW_OPERATOR = "+=";
	const std::string ParserOperatorDecrement::KW_OPERATOR = "--";
	const std::string ParserOperatorConditionalA::KW_OPERATOR = "?";
	const std::string ParserOperatorConditionalB::KW_OPERATOR = ":";
	const std::string ParserOperatorConditionalNull::KW_OPERATOR = "?:";

#define OP_PARSECHAIN_BLOCK(P_OP) { \
	auto op = std::make_shared<P_OP>();\
	ParseResult res = ParserWord::parse(itrange, op->opchar, op/*, true*/); \
	if (res.isOk()) {\
		this->opchar = op->getChar();\
		return res;\
	}\
}

#define OP_SINGLE_PARSECHAIN_BLOCK(P_OP, POS) { \
	auto op = std::make_shared<P_OP>(POS);\
	ParseResult res = ParserWord::parse(itrange, op->opchar, op/*, true*/); \
	if (res.isOk()) {\
		this->opchar = op->getChar();\
		return res;\
	}\
}

	ParserOperator::ParserOperator(const char *kw, OPERATOR_POSITION_T position)
		:opchar(kw),
		position(position)
	{
	}

	ParserOperator::ParserOperator(OPERATOR_POSITION_T position)
		: position(position)
	{
	}

	const std::string &ParserOperator::getChar() const
	{
		return this->opchar;
	}

	ParseResult ParserOperator::parse(StringIteratorRange itrange)
	{
		OP_SINGLE_PARSECHAIN_BLOCK(ParserOperatorIncrement, this->position);
		OP_SINGLE_PARSECHAIN_BLOCK(ParserOperatorDecrement, this->position);
		OP_PARSECHAIN_BLOCK(ParserOperatorConditionalNull);
		OP_PARSECHAIN_BLOCK(ParserOperatorConditionalA);
		OP_PARSECHAIN_BLOCK(ParserOperatorConditionalB);
		OP_PARSECHAIN_BLOCK(ParserOperatorMemberAccessor);
		OP_PARSECHAIN_BLOCK(ParserOperatorEquals);
		OP_PARSECHAIN_BLOCK(ParserOperatorAssign);
		OP_PARSECHAIN_BLOCK(ParserOperatorAddTo);
		OP_PARSECHAIN_BLOCK(ParserOperatorAdd);
		OP_PARSECHAIN_BLOCK(ParserOperatorSubtractFrom);
		OP_PARSECHAIN_BLOCK(ParserOperatorSubtract);
		OP_PARSECHAIN_BLOCK(ParserOperatorMultiplyBy);
		OP_PARSECHAIN_BLOCK(ParserOperatorMultiply);
		OP_PARSECHAIN_BLOCK(ParserOperatorDivideBy);
		OP_PARSECHAIN_BLOCK(ParserOperatorDivide);
		OP_PARSECHAIN_BLOCK(ParserOperatorGreaterThanOrEqual);
		OP_PARSECHAIN_BLOCK(ParserOperatorGreaterThan);
		OP_PARSECHAIN_BLOCK(ParserOperatorLessThanOrEqual);
		OP_PARSECHAIN_BLOCK(ParserOperatorLessThan);

		return ParseResult(ParseResult::STATUS_T::S_FATAL);
	}
}