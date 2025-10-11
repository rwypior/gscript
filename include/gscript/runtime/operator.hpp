#ifndef _h_gscript_operator
#define _h_gscript_operator

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "factory/OperatorFunctionFactory.hpp"

#include <memory>

namespace gscript
{
	class ScriptVarRead;

	class ScriptOperator : public ScriptCallable
	{
	public:
		static std::unordered_map<OperatorType, std::string> opmap;

		OperatorLinkage linkage;
		std::shared_ptr<ScriptCallable> left = nullptr;
		std::shared_ptr<ScriptCallable> right = nullptr;

		ScriptOperator(ScriptScope &scope, OperatorLinkage linkage = OperatorLinkage::Single);

		virtual const std::shared_ptr<ScriptType> getType() const override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override = 0;
		virtual const int getPrecedence() const = 0;
		virtual const OperatorType getOperatorType() const = 0;
		virtual bool needFactory() const;
		virtual void setup() {};

		void assignOperatorFunction();

		static std::string translateOperator(OperatorType t);

	protected:
		OperatorFunctionFactory::OperatorFunction operatorFunction = nullptr;
	};

	class ScriptOperatorMemberAccessor : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorNegate : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAssign : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;

	protected:
		ScriptVarRead *varRead = nullptr;
	};

	class ScriptOperatorEquals : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThan : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThan : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThanOrEqual : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThanOrEqual : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAdd : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAddTo : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtract : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtractFrom : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiply : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiplyBy : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivide : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivideBy : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorIncrement : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDecrement : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorConditionalA : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorConditionalB : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;
	};

	class ScriptOperatorConditionalNull : public ScriptOperator
	{
	public:
		using ScriptOperator::ScriptOperator;
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};
}

#endif