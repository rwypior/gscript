#include "parser/pOperator.hpp"
#include "parser/pWord.hpp"
#include "StringUtils.hpp"

#include <sstream>

namespace gscript
{
#define OP_PARSECHAIN_BLOCK(P_OP) { \
	auto op = std::make_shared<P_OP>();\
	ParseResult res = ParserWord::parseExact(itrange, op->opchar, op); \
	if (res.isOk()) {\
		this->opchar = op->getChar();\
		return res;\
	}\
}

#define OP_SINGLE_PARSECHAIN_BLOCK(P_OP, POS) { \
	auto op = std::make_shared<P_OP>(POS);\
	ParseResult res = ParserWord::parseExact(itrange, op->opchar, op); \
	if (res.isOk()) {\
		this->opchar = op->getChar();\
		return res;\
	}\
}

	ParserOperator::ParserOperator(const char *kw, OperatorPosition position)
		:opchar(kw),
		position(position)
	{
	}

	ParserOperator::ParserOperator(OperatorPosition position)
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

		return ParseResult(ParseResult::Status::Invalid, { itrange, (std::stringstream() << "Expected operator, got \"" << getCharsUntilEol(itrange.begin, itrange.end) << "\"").str()});
	}
}