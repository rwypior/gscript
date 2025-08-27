#include "parser/pFunction.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pTypeSpecifier.hpp"
#include "parser/pBlockStart.hpp"
#include "parser/pBlockEnd.hpp"
#include "parser/pArglist.hpp"
#include "parser/pVarDeclaration.hpp"
#include "parser/pStatement.hpp"
#include "parser/pReturn.hpp"
#include "parser/pConstructor.hpp"
#include "parser/pOperator.hpp"
#include "parser/pExternSpecial.hpp"
#include "parser/pAbstractSpecial.hpp"

#include "compileException.hpp"

namespace gscript
{
	ParseResult ParserFunction::parse(StringIteratorRange itrange)
	{
		bool isConstructor = dynamic_cast<ParserConstructor*>(this) != NULL;

		auto typeBegin = itrange.begin;
		auto typeEnd = itrange.begin;

		if (!isConstructor)
		{
			ParserTypeSpecifier type = ParserTypeSpecifier();
			ParseResult typeres = type.parse(itrange);
			if (!typeres.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			typeBegin = typeres.result.begin;
			typeEnd = typeres.result.end + 1;

			this->returnTypeName = typeres.getWord();
			this->returnTypeClassifier = ScriptType::translateType(this->returnTypeName);
		}
		else
		{
			this->returnTypeName = "";
			this->returnTypeClassifier = VALUE_TYPE_T::VT_CLASS;
		}

		ParseResult name = (ParserNameSpecifier()).parse(StringIteratorRange(typeEnd, itrange.end));
		if (!name.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		this->name = name.getWord();

		ParseResult arglist = this->arglist.parse(StringIteratorRange(name.result.end, itrange.end));
		if (!arglist.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		ParseResult assign = (ParserOperatorAssign()).parse(StringIteratorRange(arglist.result.end, itrange.end));

		if (assign.isOk())
		{
			ParserExternSpecial pExtern;
			ParseResult externResult = pExtern.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (externResult.isOk())
			{
				this->externName = pExtern.name;
				return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(typeBegin, externResult.result.end));
			}

			ParserAbstractSpecial pAbstract;
			ParseResult abstractResult = pAbstract.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (abstractResult.isOk())
			{
				if (ParserMethod *method = dynamic_cast<ParserMethod*>(this))
				{
					method->accessSpecifier.modifier |= MODIFIER_T::M_ABSTRACT;
				}
				else
					throw CompileException("Only methods may be declared abstract");

				//this->isabstract = true;
				return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(typeBegin, abstractResult.result.end));
			}

			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
		}
		else
		{
			ParseResult begin = (ParserBlockStart()).parse(StringIteratorRange(arglist.result.end + 1, itrange.end));
			if (!begin.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			ParseResult bodyres = this->body.parse(StringIteratorRange(begin.result.end, itrange.end));
			if (!bodyres.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			ParseResult end = (ParserBlockEnd()).parse(StringIteratorRange(bodyres.result.end, itrange.end));
			if (!end.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

			return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(typeBegin, end.result.end));
		}
	}
}