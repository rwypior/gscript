#include <iostream>

#include "pFunction.hpp"
#include "pNameSpecifier.hpp"
#include "pTypeSpecifier.hpp"
#include "pBlockStart.hpp"
#include "pBlockEnd.hpp"
#include "pArglist.hpp"
#include "pVarDeclaration.hpp"
#include "pStatement.hpp"
#include "pReturn.hpp"
#include "pConstructor.hpp"
#include "pOperator.hpp"

#include "pExternSpecial.hpp"
#include "pAbstractSpecial.hpp"

#include "compileException.hpp"

namespace gscript
{
	ParseResult ParserFunction::parse(StringIteratorRange itrange)
	{
		bool isConstructor = dynamic_cast<ParserConstructor*>(this) != NULL;

		std::string::iterator typeBegin = itrange.begin;
		std::string::iterator typeEnd = itrange.begin;

		if (!isConstructor)
		{
			ParserTypeSpecifier type = ParserTypeSpecifier();
			ParseResult typeres = type.parse(itrange);
			if (!typeres.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

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
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		this->name = name.getWord();

		ParseResult arglist = this->arglist.parse(StringIteratorRange(name.result.end, itrange.end));
		if (!arglist.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

		ParseResult assign = (ParserOperatorAssign()).parse(StringIteratorRange(arglist.result.end, itrange.end));

		if (assign.isOk())
		{
			ParserExternSpecial pExtern;
			ParseResult externResult = pExtern.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (externResult.isOk())
			{
				this->externName = pExtern.name;
				return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(typeBegin, externResult.result.end));
			}

			ParserAbstractSpecial pAbstract;
			ParseResult abstractResult = pAbstract.parse(StringIteratorRange(assign.result.end, itrange.end));

			if (abstractResult.isOk())
			{
				if (ParserMethod *method = dynamic_cast<ParserMethod*>(this))
				{
					method->accessSpecifier.modifier |= static_cast<BITFLAG_T>(MODIFIER_T::M_ABSTRACT);
				}
				else
					throw CompileException("Only methods may be declared abstract");

				//this->isabstract = true;
				return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(typeBegin, abstractResult.result.end));
			}

			return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());
		}
		else
		{
			ParseResult begin = (ParserBlockStart()).parse(StringIteratorRange(arglist.result.end + 1, itrange.end));
			if (!begin.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

			ParseResult bodyres = this->body.parse(StringIteratorRange(begin.result.end, itrange.end));
			if (!bodyres.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

			ParseResult end = (ParserBlockEnd()).parse(StringIteratorRange(bodyres.result.end, itrange.end));
			if (!end.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, ParserEntity::StringIteratorRange());

			return ParseResult(ParseResult::STATUS_T::S_OK, ParserEntity::StringIteratorRange(typeBegin, end.result.end));
		}
	}
}