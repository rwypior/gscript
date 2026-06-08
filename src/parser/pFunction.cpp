#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pNameSpecifier.hpp"
#include "gscript/parser/pTypeSpecifier.hpp"
#include "gscript/parser/pBlockStart.hpp"
#include "gscript/parser/pBlockEnd.hpp"
#include "gscript/parser/pArglist.hpp"
#include "gscript/parser/pVarDeclaration.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pConstructor.hpp"
#include "gscript/parser/pOperator.hpp"
#include "gscript/parser/pExternSpecial.hpp"
#include "gscript/parser/pAbstractSpecial.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/parser/pOperatorDeclaration.hpp"

#include "gscript/compileException.hpp"

namespace gscript
{
	ParseResult ParserFunction::parse(StringIteratorRange itrange)
	{
		bool isConstructor = dynamic_cast<ParserConstructor*>(this) != nullptr;

		auto typeBegin = itrange.begin;
		auto typeEnd = itrange.begin;

		if (!isConstructor)
		{
			ParserTypeSpecifier type = ParserTypeSpecifier();
			itrange.begin = parseComment(itrange.begin, itrange.end);
			ParseResult typeres = type.parse(itrange);
			if (!typeres.isOk())
				return typeres;

			typeBegin = typeres.result.begin;
			typeEnd = typeres.result.end + 1;

			this->returnTypeName = typeres.getWord();
			this->returnTypeClassifier = ScriptType::translateType(this->returnTypeName);
		}
		else
		{
			this->returnTypeName = "";
			this->returnTypeClassifier = ValueType::Class;
		}

		typeEnd = parseComment(typeEnd, itrange.end);

		ParserOperatorDeclaration oper;

		ParseResult nameResult = oper.parse(StringIteratorRange(typeEnd, itrange.end));
		if (nameResult.isOk())
		{
			this->operatorType = oper.operatorType;
		}
		else if (!nameResult.isFatal())
		{
			nameResult = (ParserNameSpecifier()).parse(StringIteratorRange(typeEnd, itrange.end));
			if (!nameResult.isOk())
				return nameResult;
		}
		else
			return nameResult;

		this->name = nameResult.getWord();

		ParseResult arglist = this->arglist.parse(StringIteratorRange(nameResult.result.end, itrange.end));
		if (!arglist.isOk())
			return arglist;

		arglist.result.end = parseComment(arglist.result.end, itrange.end);
		ParseResult assign = (ParserOperatorAssign()).parse(StringIteratorRange(arglist.result.end, itrange.end));

		if (assign.isOk())
		{
			ParserExternSpecial pExtern;
			ParseResult externResult = pExtern.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (externResult.isOk())
			{
				this->externName = pExtern.name;
				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(typeBegin, externResult.result.end));
			}

			ParserAbstractSpecial pAbstract;
			ParseResult abstractResult = pAbstract.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (abstractResult.isOk())
			{
				if (ParserMethod *method = dynamic_cast<ParserMethod*>(this))
					method->accessSpecifier.modifier |= Modifier::Abstract;
				else
					throw CompileException("Only methods may be declared abstract");

				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(typeBegin, abstractResult.result.end));
			}

			return ParseResult(ParseResult::Status::Fatal, { itrange, "Expected extern declaration or abstract specifier" });
		}
		else
		{
			ParseResult begin = (ParserBlockStart()).parse(StringIteratorRange(arglist.result.end + 1, itrange.end));
			if (!begin.isOk())
				return begin.as(ParseResult::Status::Fatal);

			ParseResult bodyres = this->body.parse(StringIteratorRange(begin.result.end, itrange.end));
			if (!bodyres.isOk())
				return bodyres.as(ParseResult::Status::Fatal);

			ParseResult end = (ParserBlockEnd()).parse(StringIteratorRange(bodyres.result.end, itrange.end));
			if (!end.isOk())
				return end.as(ParseResult::Status::Fatal);

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(typeBegin, end.result.end));
		}
	}
}