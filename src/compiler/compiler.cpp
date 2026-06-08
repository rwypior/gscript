#include "gscript/compiler/compiler.hpp"

#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pBlockBody.hpp"
#include "gscript/parser/pNamespace.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pMethod.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pNew.hpp"
#include "gscript/parser/pArrayInitializer.hpp"
#include "gscript/parser/pOperator.hpp"
#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pIf.hpp"
#include "gscript/parser/pElse.hpp"
#include "gscript/parser/pWhile.hpp"
#include "gscript/parser/pFor.hpp"
#include "gscript/parser/pReturn.hpp"

#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcParam.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/new.hpp"
#include "gscript/runtime/arrayInitializer.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/if.hpp"
#include "gscript/runtime/while.hpp"
#include "gscript/runtime/for.hpp"
#include "gscript/runtime/return.hpp"
#include "gscript/logger.hpp"

#include "gscript/runtimeException.hpp"
#include "gscript/compileException.hpp"

#include <unordered_map>
#include <functional>
#include <memory>

namespace gscript
{
	std::unique_ptr<FunctionParameterContainer> Compiler::compileParameterContainer(ScriptScope* scope, const ParserCallArglist& arglist)
	{
		gs_log("Compiling FunctionParameterContainer");

		std::vector<std::unique_ptr<ScriptStatement>> statements;
		statements.reserve(arglist.parameters.size());
		for (auto& param : arglist.parameters)
		{
			statements.emplace_back(this->compileStatement(scope, *param));
		}
		return std::make_unique<FunctionParameterContainer>(std::move(statements));
	}

	std::unique_ptr<ScriptNamespace> Compiler::compileNamespace(ScriptScope* scope, const ParserNamespace& entry)
	{
		gs_log("Compiling ScriptNamespace " << entry.name);

		auto ns = std::make_unique<ScriptNamespace>(scope, entry.name);

		for (auto& cls : entry.classes)
		{
			ns->registerClass(this->compileClass(ns.get(), cls));
		}

		for (auto& fnc : entry.functions)
		{
			ns->registerFunction(this->compileFunction(scope, fnc));
		}

		return ns;
	}

	std::unique_ptr<ScriptClass> Compiler::compileClass(ScriptNamespace* scope, const ParserClass& pclass)
	{
		gs_log("Compiling ScriptClass " << pclass.name);

		const std::string& base = pclass.base;

		ScriptClass* baseClass = nullptr;

		if (base.length() > 0)
		{
			baseClass = scope->findClass(base);
		}

		auto cl = std::make_unique<ScriptClass>(*scope, pclass.name, baseClass);

		for (auto& pf : pclass.fields)
		{
			//auto field = std::make_unique<ScriptFieldDeclaration>(pf.name, ScriptType::create(pf.type, *cl), this->compileStatement(cl.get(), pf.value));
			auto field = std::make_unique<ScriptFieldDeclarationPrototype>(*cl, pf.name, ScriptType::create(pf.type, *cl), this->compileStatement(cl.get(), pf.value));
			cl->addFieldDeclaration(std::move(field));
		}

		for (auto& pmethod : pclass.methods)
		{
			auto method = this->compileMethod(cl.get(), pmethod);
			cl->registerFunction(std::move(method));
		}

		return cl;
	}

	std::unique_ptr<ScriptFunction> Compiler::compileFunction(ScriptScope* scope, const ParserFunction& pfunc)
	{
		gs_log("Compiling ScriptFunction " << pfunc.name);

		PARAMS_T params;
		for (auto& pp : pfunc.arglist.parameters)
		{
			params.emplace_back(ScriptType::create(pp.type, *scope), pp.name);
		}

		auto newfunc = std::make_unique<ScriptFunction>(*scope, pfunc.name, ScriptType::create(pfunc.returnTypeName, *scope), params);
		newfunc->merge(std::move(this->compileExecutiveBlock(newfunc.get(), pfunc.body)));

		return newfunc;
	}

	std::unique_ptr<ScriptMethod> Compiler::compileMethod(ScriptScope* scope, const ParserMethod& pfunc)
	{
		gs_log("Compiling ScriptMethod " << pfunc.name);

		PARAMS_T params;
		for (auto& pp : pfunc.arglist.parameters)
		{
			params.emplace_back(ScriptType::create(pp.type, *scope), pp.name);
		}

		auto newfunc = std::make_unique<ScriptMethod>(*scope, pfunc.name, ScriptType::create(pfunc.returnTypeName, *scope), params, pfunc.accessSpecifier.getModifier());
		newfunc->merge(std::move(this->compileExecutiveBlock(newfunc.get(), pfunc.body)));

		return newfunc;
	}

	bool parseBool(const std::string& value)
	{
		if (value == "true")
			return true;
		else if (value == "false")
			return false;

		return std::stoi(value);
	}

	std::unique_ptr<ScriptValue> Compiler::compileValue(const ParserLiteral& pLiteral)
	{
		gs_log("Compiling ScriptValue " << pLiteral.value);

		switch (pLiteral.type)
		{
		case ValueType::Bool:
			return std::make_unique<ScriptBoolValue>(parseBool(pLiteral.value));
		case ValueType::Char:
			return std::make_unique<ScriptCharValue>(pLiteral.value[0]);
		case ValueType::Byte:
			return std::make_unique<ScriptByteValue>(static_cast<unsigned char>(std::stoul(pLiteral.value)));
		case ValueType::Int:
			return std::make_unique<ScriptIntValue>(std::stoi(pLiteral.value));
		case ValueType::UnsignedInt:
			return std::make_unique<ScriptUnsignedIntValue>(static_cast<unsigned int>(std::stoul(pLiteral.value)));
		case ValueType::Float:
			return std::make_unique<ScriptFloatValue>(std::stof(pLiteral.value));
		case ValueType::Double:
			return std::make_unique<ScriptDoubleValue>(std::stod(pLiteral.value));
		case ValueType::String:
			return std::make_unique<ScriptStringValue>(pLiteral.value);
		}

		throw RuntimeException(std::string("Unknown type ID ") + std::to_string(static_cast<size_t>(pLiteral.type)));
	}

	std::unique_ptr<ScriptStatement> Compiler::compileStatement(ScriptScope* scope, const ParserStatement& pstatement, bool topmostVarReadReference)
	{
		gs_log("Compiling ScriptStatement " << pstatement.components.size() << " components");

		std::vector<std::unique_ptr<ScriptCallable>> substatements;
		substatements.reserve(pstatement.components.size());
		int currentIndex = 0;

		ScriptScopeBase* stmtScope = nullptr;

		for (auto& comp : pstatement.components)
		{
			ScriptScopeBase* currentScope = stmtScope ? stmtScope : scope;

			if (auto varread = std::dynamic_pointer_cast<ParserVar>(comp))
			{
				substatements.push_back(this->compileVarRead(scope, *varread));
			}
			else if (auto lit = std::dynamic_pointer_cast<ParserLiteral>(comp))
			{
				substatements.push_back(std::make_unique<ScriptLiteral>(this->compileValue(*lit)));
			}
			else if (auto stmt = std::dynamic_pointer_cast<ParserStatement>(comp))
			{
				substatements.push_back(this->compileStatement(scope, *stmt));
			}
			else if (auto fcall = std::dynamic_pointer_cast<ParserNew>(comp))
			{
				substatements.push_back(this->compileNewCall(scope, *fcall));
			}
			else if (auto fcall = std::dynamic_pointer_cast<ParserFuncCall>(comp))
			{
				substatements.push_back(this->compileFuncCall(scope, *fcall));
			}
			else if (auto arr = std::dynamic_pointer_cast<ParserArrayInitializer>(comp))
			{
				substatements.push_back(this->compileArrayInitializer(scope, *arr));
			}
			else if (auto oper = std::dynamic_pointer_cast<ParserOperator>(comp))
			{
				substatements.push_back(this->compileOperator(*oper));
			}
			else
				throw CompileException("Unknown entity type");
		}

		if (!topmostVarReadReference && substatements.size() == 1)
		{
			if (auto varread = dynamic_cast<ScriptVarReadPrototype*>(substatements.front().get()))
			{
				varread->disableReference();
			}
		}

		return std::make_unique<ScriptStatement>(std::move(substatements));
	}

	std::unique_ptr<ScriptExecutiveBlock> Compiler::compileExecutiveBlock(ScriptScope* scope, const ParserBlockBody& pblock)
	{
		gs_log("Compiling ScriptExecutiveBlock with " << pblock.statements.size() << " statements");

		std::vector<std::unique_ptr<ScriptCallable>> statements;

		for (auto &ps : pblock.statements)
		{
			if (auto statement = std::dynamic_pointer_cast<ParserVarDeclaration>(ps))
				statements.push_back(this->compileVarDeclaration(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserStatement>(ps))
				statements.push_back(this->compileStatement(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserReturn>(ps))
				statements.push_back(this->compileReturn(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserIf>(ps))
				statements.push_back(this->compileIf(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserWhile>(ps))
				statements.push_back(this->compileWhile(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserFor>(ps))
				statements.push_back(this->compileFor(scope, *statement));
		}

		return std::make_unique<ScriptExecutiveBlock>(std::move(statements));
	}

	std::unique_ptr<ScriptOperator> Compiler::compileOperator(const ParserOperator& pOperator)
	{
		gs_log("Compiling ScriptOperator " << pOperator.getChar());

		std::unordered_map<OperatorType, std::function<std::unique_ptr<ScriptOperator>(OperatorLinkage)>> opmap = {
			{ OperatorType::MemberAccessor, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorMemberAccessor>(l); }},
			{ OperatorType::Add, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorAdd>(l); }},
			{ OperatorType::AddTo, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorAddTo>(l); }},
			{ OperatorType::Subtract, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorSubtract>(l); }},
			{ OperatorType::SubtractFrom, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorSubtractFrom>(l); }},
			{ OperatorType::Multiply, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorMultiply>(l); }},
			{ OperatorType::MultiplyBy, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorMultiplyBy>(l); }},
			{ OperatorType::Divide, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorDivide>(l); }},
			{ OperatorType::DivideBy, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorDivideBy>(l); }},
			{ OperatorType::Equals, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorEquals>(l); }},
			{ OperatorType::NotEquals, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorEquals>(l); }}, // TODO - missing operator for not equals?
			{ OperatorType::GreaterThan, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorGreaterThan>(l); }},
			{ OperatorType::GreaterThanOrEqual, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorGreaterThanOrEqual>(l); }},
			{ OperatorType::LesserThan, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorLessThan>(l); }},
			{ OperatorType::LesserThanOrEqual, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorLessThanOrEqual>(l); }},
			{ OperatorType::Assign, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorAssign>(l); }},
			{ OperatorType::Negate, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorNegate>(l); }},
			{ OperatorType::Increment, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(l); }},
			{ OperatorType::PreIncrement, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(l); }},
			{ OperatorType::PostIncrement, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(l); }},
			{ OperatorType::Decrement, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(l); }},
			{ OperatorType::PreDecrement, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(l); }},
			{ OperatorType::PostDecrement, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(l); }},
			{ OperatorType::ConditionalIf, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalA>(l); }},
			{ OperatorType::ConditionalElse, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalB>(l); }},
			{ OperatorType::ConditionalNull, [](OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalNull>(l); }}
		};

		auto it = opmap.find(pOperator.getType());
		if (it == opmap.end())
			throw CompileException("Cannot find operator " + std::to_string(static_cast<int>(pOperator.getType())));

		return it->second(pOperator.getLinkage());
	}

	std::unique_ptr<ScriptCallable> Compiler::compileVarRead(ScriptScope* scope, const ParserVar& pVar)
	{
		gs_log("Compiling ScriptVarRead " << pVar.name.getString());

		if (auto& arr = pVar.arrayAccessor)
			return std::make_unique<ScriptArrayReadPrototype>(pVar.name, this->compileStatement(scope, arr->statement));

		return std::make_unique<ScriptVarReadPrototype>(pVar.name);
	}

	std::unique_ptr<ScriptNew> Compiler::compileNewCall(ScriptScope* scope, const ParserNew& fcall)
	{
		gs_log("Compiling ScriptNew " << fcall.name.getString());

		auto params = this->compileParameterContainer(scope, fcall.arglist);

		ScriptNamespace* ns = scope->getGlobalNamespace();
		ScriptClass* sclass = ns->findClass(fcall.constructorName);

		if (!sclass)
			throw CompileException(std::string("Unable to find class \"" + fcall.constructorName + "\""));

		return std::make_unique<ScriptNew>(*sclass, std::move(params->getParams()));
	}

	std::unique_ptr<ScriptCallable> Compiler::compileFuncCall(ScriptScope* scope, const ParserFuncCall& fcall)
	{
		gs_log("Compiling ScriptFuncCall " << fcall.name.getString());

		auto params = this->compileParameterContainer(scope, fcall.arglist);

		ScriptScope* usedScope = scope;
		std::string usedName = fcall.name;

		if (fcall.name.isScoped())
		{
			EntityPath::scope_iterator scopeIt = fcall.name.createScopeIterator();

			ScriptNamespace* ns = scope->getGlobalNamespace();
			for (; scopeIt != fcall.name.getString().end(); ++scopeIt)
			{
				if (scopeIt.last())
				{
					usedName = *scopeIt;
				}
				else
				{
					ScriptNamespace* foundns = ns->findClass(*scopeIt);

					if (!foundns)
					{
						foundns = ns->findNamespace(*scopeIt);

						if (!foundns)
							throw CompileException(std::string("Class \"" + *scopeIt + "\" was not found"));
					}

					usedScope = foundns;
				}
			}
		}

		bool isStaticCall = fcall.name.isScoped();
		return std::make_unique<ScriptFuncCallPrototype>(isStaticCall ? usedScope : nullptr, usedName, std::move(params->getParams()), isStaticCall);
	}

	std::unique_ptr<ScriptArrayInitializer> Compiler::compileArrayInitializer(ScriptScope* scope, const ParserArrayInitializer& initializer)
	{
		gs_log("Compiling ScriptArrayInitializer with " << initializer.arglist.parameters.size() << " arguments");

		std::shared_ptr<ScriptType> prevType;
		std::vector<std::unique_ptr<ScriptStatement>> statements;
		
		for (auto& param : initializer.arglist.parameters)
		{
			auto stmt = this->compileStatement(scope, *param);
			const auto type = stmt->getType();

			if (prevType && !prevType->equals(*type))
				throw CompileException("Mixed-type array not allowed");

			prevType = type;
			statements.push_back(std::move(stmt));
		}

		auto type = new ScriptArrayType(prevType ? prevType : ScriptType::createNull());

		return std::make_unique<ScriptArrayInitializer>(std::move(statements));
	}

	std::unique_ptr<ScriptVarDeclaration> Compiler::compileVarDeclaration(ScriptScope* scope, const ParserVarDeclaration& pVar)
	{
		gs_log("Compiling ScriptVarDeclaration " << pVar.name);

		auto& v = scope->registerVariable(std::make_unique<ScriptVariable>(pVar.name, ScriptType::create(pVar.type, *scope), nullptr));
		auto stmt = this->compileStatement(scope, pVar.value);
		return std::make_unique<ScriptVarDeclaration>(*scope, v, std::move(stmt));
	}

	std::unique_ptr<ScriptIf> Compiler::compileIf(ScriptScope* scope, const ParserIf& pIf)
	{
		gs_log("Compiling ScriptIf");

		auto condition = this->compileStatement(scope, *pIf.arglist.parameters.front());

		std::unique_ptr<ScriptIf> selse;
		if (!pIf.pelse.body.body.statements.empty())
			selse = this->compileElse(scope, pIf.pelse);

		auto sif = std::make_unique<ScriptIf>(*scope, std::move(condition), std::move(selse));
		auto exeblock = this->compileExecutiveBlock(sif.get(), pIf.body.body);
		sif->merge(std::move(exeblock));

		return sif;
	}

	std::unique_ptr<ScriptIf> Compiler::compileElse(ScriptScope* scope, const ParserElse& pElse)
	{
		gs_log("Compiling ScriptElse");

		std::unique_ptr<ScriptStatement> condition;
		std::unique_ptr<ScriptIf> selse;
		if (pElse.pif)
		{
			condition = this->compileStatement(scope, *pElse.pif->arglist.parameters.front());
			if (!pElse.pif->pelse.body.body.statements.empty())
				selse = this->compileElse(scope, pElse.pif->pelse);
		}
		
		auto sif = std::make_unique<ScriptIf>(*scope, std::move(condition), std::move(selse));
		auto exeblock = this->compileExecutiveBlock(sif.get(), pElse.body.body);
		sif->merge(std::move(exeblock));

		return sif;
	}

	std::unique_ptr<ScriptWhile> Compiler::compileWhile(ScriptScope* scope, const ParserWhile& pWhile)
	{
		gs_log("Compiling ScriptWhile");

		// TODO - make while have it's own scope, and place any variable created by
		// condition into that scope

		auto condition = this->compileStatement(scope, *pWhile.arglist.parameters.front());
		auto exeblock = this->compileExecutiveBlock(scope, pWhile.body.body);

		auto swhile = std::make_unique<ScriptWhile>(*scope, std::move(condition));
		swhile->merge(std::move(exeblock));

		return swhile;
	}

	std::unique_ptr<ScriptFor> Compiler::compileFor(ScriptScope* scope, const ParserFor& pFor)
	{
		gs_log("Compiling ScriptFor");

		// TODO - make for have it's own scope, and place the vardecl into that scope

		auto vardecl = this->compileVarDeclaration(scope, pFor.arglist.varDecl);
		auto condition = this->compileStatement(scope, pFor.arglist.condition);
		auto progress = this->compileStatement(scope, pFor.arglist.progress);
		auto exeblock = this->compileExecutiveBlock(scope, pFor.body.body);

		auto sfor = std::make_unique<ScriptFor>(*scope, std::move(vardecl), std::move(condition), std::move(progress));
		sfor->merge(std::move(exeblock));

		return sfor;
	}

	std::unique_ptr<ScriptReturn> Compiler::compileReturn(ScriptScope* scope, const ParserReturn& pReturn)
	{
		gs_log("Compiling ScriptReturn with " << pReturn.value.components.size() << " components");

		auto stmt = this->compileStatement(scope, pReturn.value, false);
		return std::make_unique<ScriptReturn>(std::move(stmt));
	}

	// Finalize

	void Compiler::finalize(ScriptNamespace& ns)
	{
		gs_log("Compiler - finalize");

		this->finalizeScope(ns);

		for (auto& ns : ns.getNamespaces())
		{
			this->finalize(*ns);
			this->finalizeScope(*ns);
		}

		for (auto& cls : ns.getClasses())
		{
			this->finalizeClass(cls);
			this->finalizeScope(*cls);
		}
	}

	void Compiler::finalizeScope(ScriptScopeBase& scope)
	{
		for (auto& fnc : scope.getFunctions())
		{
			auto& stmts = fnc->getStatements();

			for (auto& stmt : fnc->getStatements())
			{
				this->finalizeCallable(stmt, *fnc);
			}
		}
	}

	void Compiler::finalizeClass(std::unique_ptr<ScriptClass>& cls)
	{
		for (auto& field : cls->getFields())
		{
			if (auto proto = dynamic_cast<ScriptFieldDeclarationPrototype*>(field.get()))
			{
				auto built = proto->build(*cls);
				//ScriptFieldDeclaration(*static_cast<ScriptFieldDeclaration*>(built.get()));
				field = std::make_unique<ScriptFieldDeclaration>(*static_cast<ScriptFieldDeclaration*>(built.release()));
				//field = proto->build(*cls);
			}
			/*std::unique_ptr<ScriptCallable> clb = std::move(field);
			this->finalizeCallable(clb, *cls);*/
		}
	}

	// TODO - unify this

	void Compiler::finalizeCallable(std::shared_ptr<ScriptCallable>& callable, ScriptScopeBase& scope)
	{
		if (auto proto = std::dynamic_pointer_cast<ScriptFuncCallPrototype>(callable))
		{
			for (auto& param : proto->getParams())
			{
				this->finalizeCallable(param->callable, scope);
			}
		}
		
		if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(callable))
		{
			callable = proto->build(scope);
			this->finalizeCallable(callable, scope);
		}
		else if (auto stmt = std::dynamic_pointer_cast<ScriptStatement>(callable))
		{
			if (auto proto = std::dynamic_pointer_cast<ScriptFuncCallPrototype>(stmt->callable))
			{
				for (auto& param : proto->getParams())
				{
					this->finalizeCallable(param->callable, scope);
				}
			}

			if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(stmt->callable))
			{
				stmt->callable = proto->build(scope);
			}
			this->finalizeCallable(stmt->callable, scope);
		}
		else if (auto ret = std::dynamic_pointer_cast<ScriptReturn>(callable))
		{
			this->finalizeCallable(ret->getStatement(), scope);
		}
		else if (auto oper = std::dynamic_pointer_cast<ScriptOperator>(callable))
		{
			this->finalizeCallable(oper->left, scope);

			ScriptScopeBase* usedScope = &scope;
			if (auto memberacc = std::dynamic_pointer_cast<ScriptOperatorMemberAccessor>(oper))
			{
				auto& target = std::static_pointer_cast<const ScriptClassType>(memberacc->left->getType())->getClass();
				usedScope = &target;
			}

			this->finalizeCallable(oper->right, *usedScope);

			if (oper->needFactory())
				oper->assignOperatorFunction();
		}
		else if (auto vardecl = std::dynamic_pointer_cast<ScriptVarDeclaration>(callable))
		{
			this->finalizeCallable(vardecl->getStatement(), scope);
		}
		else if (auto sif = std::dynamic_pointer_cast<ScriptIf>(callable))
		{
			this->finalizeCallable(sif->getCondition(), scope);
			for (auto& stmt : sif->getStatements())
			{
				this->finalizeCallable(stmt, *sif);
			}
		}
	}

	void Compiler::finalizeCallable(std::unique_ptr<ScriptCallable>& callable, ScriptScopeBase& scope)
	{
		if (auto proto = dynamic_cast<ScriptFuncCallPrototype*>(callable.get()))
		{
			for (auto& param : proto->getParams())
			{
				this->finalizeCallable(param->callable, scope);
			}
		}
		
		if (auto proto = dynamic_cast<ScriptCallablePrototype*>(callable.get()))
		{
			callable = proto->build(scope);
			this->finalizeCallable(callable, scope);
		}
		else if (auto stmt = dynamic_cast<ScriptStatement*>(callable.get()))
		{
			if (auto proto = std::dynamic_pointer_cast<ScriptFuncCallPrototype>(stmt->callable))
			{
				for (auto& param : proto->getParams())
				{
					this->finalizeCallable(param->callable, scope);
				}
			}

			if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(stmt->callable))
			{
				stmt->callable = proto->build(scope);
			}
			this->finalizeCallable(stmt->callable, scope);
		}
		else if (auto ret = dynamic_cast<ScriptReturn*>(callable.get()))
		{
			this->finalizeCallable(ret->getStatement(), scope);
		}
		else if (auto oper = dynamic_cast<ScriptOperator*>(callable.get()))
		{
			this->finalizeCallable(oper->left, scope);

			ScriptScopeBase* usedScope = &scope;
			if (auto memberacc = dynamic_cast<ScriptOperatorMemberAccessor*>(oper))
			{
				auto& target = std::static_pointer_cast<const ScriptClassType>(memberacc->left->getType())->getClass();
				usedScope = &target;
			}

			this->finalizeCallable(oper->right, *usedScope);

			if (oper->needFactory())
				oper->assignOperatorFunction();
		}
		else if (auto vardecl = dynamic_cast<ScriptVarDeclaration*>(callable.get()))
		{
			this->finalizeCallable(vardecl->getStatement(), scope);
		}
		else if (auto sif = dynamic_cast<ScriptIf*>(callable.get()))
		{
			this->finalizeCallable(sif->getCondition(), scope);
			for (auto& stmt : sif->getStatements())
			{
				this->finalizeCallable(stmt, *sif);
			}
		}
	}
}