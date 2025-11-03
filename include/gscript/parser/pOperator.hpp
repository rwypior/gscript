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
		ParserOperator(OperatorPosition position);
		ParserOperator(const char *op = "", OperatorPosition position = OperatorPosition::Irrelevant);

		virtual ParseResult parse(StringIteratorRange itrange) override;

		const std::string &getChar() const;

		virtual OperatorLinkage getLinkage() const
		{
			return OperatorLinkage::Both;
		}

		virtual OperatorType getType() const
		{
			return OperatorType::Invalid;
		}

	protected:
		OperatorPosition position;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::MemberAccessor;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Negate;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Assign;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Equals;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::GreaterThan;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::LesserThan;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::GreaterThanOrEqual;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::LesserThanOrEqual;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Add;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::AddTo;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Subtract;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::SubtractFrom;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Multiply;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::MultiplyBy;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::Divide;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::DivideBy;
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

		ParserOperatorIncrement(OperatorPosition position)
			: ParserOperator(keywordOperator, position)
		{
		}

		virtual OperatorLinkage getLinkage() const override
		{
			return this->position == OperatorPosition::Left ? OperatorLinkage::Left : OperatorLinkage::Right;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorIncrement>(*this)/*, true*/);
		}

		virtual OperatorType getType() const override
		{
			return OperatorType::Increment;
		}
	};

	class ParserOperatorDecrement : public ParserOperator
	{
	public:
		static constexpr char keywordOperator[] = "--";

		ParserOperatorDecrement(OperatorPosition position)
			: ParserOperator(keywordOperator, position)
		{
		}

		virtual OperatorLinkage getLinkage() const override
		{
			return this->position == OperatorPosition::Left ? OperatorLinkage::Left : OperatorLinkage::Right;
		}

		ParseResult parse(StringIteratorRange itrange)
		{
			return ParserWord::parse(itrange, this->opchar, std::make_shared<ParserOperatorDecrement>(*this)/*, true*/);
		}

		virtual OperatorType getType() const override
		{
			return OperatorType::Decrement;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::ConditionalNull;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::ConditionalIf;
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

		virtual OperatorType getType() const override
		{
			return OperatorType::ConditionalElse;
		}
	};
}

#endif