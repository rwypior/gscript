#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pEndStatement.hpp"
#include "gscript/parser/pArglistStart.hpp"
#include "gscript/parser/pArglistEnd.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pOperator.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pNew.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/parser/pArrayInitializer.hpp"
#include "gscript/parser/pConditionalOperator.hpp"
#include "gscript/StringUtils.hpp"

#include <cassert>

namespace gscript
{
	ParserStatement::ParserStatement(bool isSubStatement, bool forceEnclosing, bool allowEmpty)
		: isSubStatement(isSubStatement)
		, forceEnclosing(forceEnclosing)
		, allowEmpty(allowEmpty)
	{
	}

	ParseResult ParserStatement::parse(StringIteratorRange itrange)
	{
		auto begin = itrange.begin;
		auto end = itrange.begin;

		unsigned int commentLength = 0;

		size_t newlines = 0;
		while (begin != itrange.end && std::isspace(*begin))
		{
			newlines += isNewLine(*begin);
			++begin;
		}

		if (itrange.end - begin < 1)
			return ParseResult(ParseResult::Status::Invalid, { itrange, "Empty statement" });

		// Check if starts with arglistStart
		bool inArglist = false;
		if (this->forceEnclosing)
		{
			begin = parseComment(begin, itrange.end);
			ParseResult arglistStart = (ParserArglistStart()).parse(StringIteratorRange(begin, itrange.end));
			if (arglistStart.isOk())
			{
				begin = arglistStart.result.end;
				end = begin;
				inArglist = true;
			}
			else
				return arglistStart;
		}

		// Scan for actual content of statement
		bool anyOk = false;
		bool prevOperator = true;
		bool isBegin = true;
		do
		{
			anyOk = false;

			end = parseComment(end, itrange.end);

			// TODO - implement this
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
				this->components.push_back(std::make_shared<ParserStatement>(std::move(statement)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { statementres.result, "Expected operator, got statement" });
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
				this->components.push_back(std::make_shared<ParserNew>(std::move(pnew)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { pnewres.result, "Expected operator, got new" });
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
				this->components.push_back(std::make_shared<ParserFuncCall>(std::move(funccall)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { funccallres.result, "Expected operator, got function call" });
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
				this->components.push_back(std::make_shared<ParserVar>(std::move(pvar)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { pvarres.result, "Expected operator, got variable" });
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
				this->components.push_back(std::make_shared<ParserLiteral>(std::move(literal)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { literalres.result, "Expected operator, got literal" });
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
				this->components.push_back(std::make_shared<ParserArrayInitializer>(std::move(arr)));
				if (!prevOperator)
					return ParseResult(ParseResult::Status::Fatal, { arrres.result, "Expected operator, got array initializer" });
				prevOperator = false;

				anyOk = true;
				isBegin = false;
				continue;
			}

			OperatorPosition pos = (isBegin || prevOperator) ? OperatorPosition::Right : OperatorPosition::Left;

			ParserOperator oper(pos);
			ParseResult operres = oper.parse(StringIteratorRange(end, itrange.end));
			if (operres.isOk())
			{
				end = operres.result.end;
				auto subOper = std::static_pointer_cast<ParserOperator>(operres.subResult);
				this->components.push_back(std::move(subOper));
				prevOperator = true;

				anyOk = true;
				isBegin = false;
				continue;
			}
		} while (anyOk);

		// If started with arglistStart then must end with arglistEnd
		if (inArglist)
		{
			begin = parseComment(begin, itrange.end);
			ParseResult arglistEnd = (ParserArglistEnd()).parse(StringIteratorRange(end, itrange.end));
			if (!arglistEnd.isOk())
				return arglistEnd.as(ParseResult::Status::Fatal);
			end = arglistEnd.result.end;
		}

		// If is not substatement must end with statementEnd
		if (!this->isSubStatement)
		{
			begin = parseComment(begin, itrange.end);
			ParseResult statementEnd = (ParserEndStatement()).parse(StringIteratorRange(end, itrange.end));
			if (!statementEnd.isOk())
			{
				if (this->components.empty())
					return statementEnd;
				else
					return statementEnd.as(ParseResult::Status::Fatal); // Only fatal for non-empty statements
			}
			end = statementEnd.result.end;
		}

		if (!this->allowEmpty && this->components.empty())
			return ParseResult(ParseResult::Status::Fatal, { itrange, "Expected non-empty statement" });

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(begin, end));
	}

	void ParserStatement::setAllowEmpty(bool allowEmpty)
	{
		this->allowEmpty = allowEmpty;
	}
}