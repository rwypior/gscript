#ifndef _h_gscript_operator
#define _h_gscript_operator

#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/factory/OperatorFunctionFactory.hpp"

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

		SCRIPT_API ScriptOperator(const ScriptOperator& b);
		SCRIPT_API ScriptOperator(OperatorLinkage linkage = OperatorLinkage::Single);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override = 0;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override = 0;
		SCRIPT_API virtual const int getPrecedence() const = 0;
		SCRIPT_API virtual const OperatorType getOperatorType() const = 0;
		SCRIPT_API virtual bool needFactory() const;
		SCRIPT_API virtual void setup() {};

		SCRIPT_API void assignOperatorFunction();

		SCRIPT_API static std::string translateOperator(OperatorType t);

	protected:
		OperatorFunctionFactory::OperatorFunction operatorFunction = nullptr;
	};

	class ScriptOperatorMemberAccessor : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
		SCRIPT_API virtual bool needFactory() const override;
	};

	class ScriptOperatorNegate : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAssign : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		ScriptOperatorAssign(const ScriptOperatorAssign& b);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
		SCRIPT_API virtual bool needFactory() const override;
		SCRIPT_API virtual void setup() override;

	protected:
		ScriptVarRead *varRead = nullptr;
	};

	class ScriptOperatorEquals : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThan : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThan : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThanOrEqual : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThanOrEqual : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAdd : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAddTo : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtract : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtractFrom : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiply : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiplyBy : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivide : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivideBy : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorIncrement : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDecrement : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorConditionalA : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
		SCRIPT_API virtual bool needFactory() const override;
	};

	class ScriptOperatorConditionalB : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
		SCRIPT_API virtual bool needFactory() const override;
		SCRIPT_API virtual void setup() override;
	};

	class ScriptOperatorConditionalNull : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual const int getPrecedence() const override;
		SCRIPT_API virtual const OperatorType getOperatorType() const;
		SCRIPT_API virtual bool needFactory() const override;
	};
}

#endif