#include "pStatement.hpp"
#include "pEndStatement.hpp"
#include "pArglistStart.hpp"
#include "pArglistEnd.hpp"
#include "pLiteral.hpp"
#include "pOperator.hpp"
#include "pVar.hpp"
#include "pFuncCall.hpp"
#include "pNew.hpp"
#include "pComment.hpp"
#include "pArrayInitializer.hpp"
#include "pConditionalOperator.hpp"

#include "utilParserWord.hpp"

#include <iostream>
#include <cassert>

namespace gscript
{
	ParserStatement::ParserStatement(bool isSubStatement, bool forceEnclosing)
		: isSubStatement(isSubStatement),
		forceEnclosing(forceEnclosing)
	{
	}

	ParseResult ParserStatement::parse(StringIteratorRange itrange)
	{
		std::string::iterator begin = itrange.begin;
		std::string::iterator end = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, begin, commentLength)

			while (*begin == ' ')
			{
				++begin;
			}

		// Check if starts with arglistStart
		bool inArglist = false;
		if (this->forceEnclosing)
		{
			ParseResult arglistStart = (ParserArglistStart()).parse(StringIteratorRange(begin, itrange.end));
			if (arglistStart.isOk())
			{
				begin = arglistStart.result.end;
				end = begin;
				inArglist = true;
			}
			else
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
		}

		// Scan for actual content of statement
		bool anyOk = false;
		bool prevOperator = true;
		bool isBegin = true;
		do
		{
			anyOk = false;

			/*ParserConditionalOperator condstatement;
			ParseResult condres = condstatement.parse(StringIteratorRange(end, itrange.end));
			if (condres.isOk())
			{
				end = condres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserConditionalOperator(condstatement)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}*/

			ParserStatement statement(true, true);
			ParseResult statementres = statement.parse(StringIteratorRange(end, itrange.end));
			if (statementres.isOk())
			{
				end = statementres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserStatement(statement)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			ParserNew pnew;
			ParseResult pnewres = pnew.parse(StringIteratorRange(end, itrange.end));
			if (pnewres.isOk())
			{
				end = pnewres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserNew(pnew)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			ParserFuncCall funccall;
			ParseResult funccallres = funccall.parse(StringIteratorRange(end, itrange.end));
			if (funccallres.isOk())
			{
				end = funccallres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserFuncCall(funccall)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			ParserVar pvar;
			ParseResult pvarres = pvar.parse(StringIteratorRange(end, itrange.end));
			if (pvarres.isOk())
			{
				end = pvarres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserVar(pvar)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			ParserLiteral literal;
			ParseResult literalres = literal.parse(StringIteratorRange(end, itrange.end));
			if (literalres.isOk())
			{
				end = literalres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserLiteral(literal)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			ParserArrayInitializer arr;
			ParseResult arrres = arr.parse(StringIteratorRange(end, itrange.end));
			if (arrres.isOk())
			{
				end = arrres.result.end;
				this->components.push_back(std::shared_ptr<ParserEntity>(new ParserArrayInitializer(arr)));
				if (!prevOperator)
					return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			OPERATOR_POSITION_T pos = (isBegin || prevOperator) ? OPERATOR_POSITION_T::OP_RIGHT : OPERATOR_POSITION_T::OP_LEFT;

			if (pos == OPERATOR_POSITION_T::OP_LEFT)
				assert(!"DEBUG");

			ParserOperator oper(pos);
			ParseResult operres = oper.parse(StringIteratorRange(end, itrange.end));
			if (operres.isOk())
			{
				end = operres.result.end;
				ParserOperator *subOper = static_cast<ParserOperator*>(operres.subResult);
				this->components.push_back(std::shared_ptr<ParserEntity>(subOper));
				prevOperator = true;

				anyOk = true;
				isBegin = false;
				continue;
			}
		} while (anyOk);

		// If started with arglistStart then must end with arglistEnd
		if (inArglist)
		{
			ParseResult arglistEnd = (ParserArglistEnd()).parse(StringIteratorRange(end, itrange.end));
			if (!arglistEnd.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
			end = arglistEnd.result.end;
		}

		// If is not substatement must end with statementEnd
		if (!this->isSubStatement)
		{
			ParseResult statementEnd = (ParserEndStatement()).parse(StringIteratorRange(end, itrange.end));
			if (!statementEnd.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
			end = statementEnd.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(begin, end));
	}

	ParserStatement::~ParserStatement()
	{
	}
}