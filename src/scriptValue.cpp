#include "scriptValue.hpp"

#include "classInstance.hpp"

namespace gscript
{
	// DEFINITIONS
#define S_VAL_OPER(VAL_OPER, VAL_RET, OPER) if (const VAL_OPER *dc = dynamic_cast<const VAL_OPER*>(&b)) return new VAL_RET(this->getValue() OPER dc->getValue());

#define SV_OP_DEF(RES_TYPE, OPER) return new RES_TYPE(this->getValue() OPER b.getValue());
#define SV_OP_NOOPER(RES_TYPE) return new RES_TYPE();
#define SV_OP_SINGLE_OPER(RES_TYPE, OPER) return new RES_TYPE(OPER this->getValue());

#define SV_OP_METHOD_NOOPER(TYPE_A, TYPE_B, RES_TYPE, OPER) \
	ScriptValue *TYPE_A::operator OPER(const TYPE_B &b) const \
	{ \
		SV_OP_NOOPER(RES_TYPE)\
	}

#define SV_OP_METHOD(TYPE_A, TYPE_B, RES_TYPE, OPER) \
	ScriptValue *TYPE_A::operator OPER(const TYPE_B &b) const \
	{ \
		SV_OP_DEF(RES_TYPE, OPER)\
	}

#define SV_OP_METHOD_SINGLE_OPER(TYPE, RES_TYPE, OPER) \
	ScriptValue *TYPE::operator OPER() const \
	{ \
		SV_OP_SINGLE_OPER(RES_TYPE, OPER)\
	}

#define SV_OP_METHOD_SINGLE_OPER_NOOPER(TYPE, RES_TYPE, OPER) \
	ScriptValue *TYPE::operator OPER() const \
	{ \
		SV_OP_NOOPER(RES_TYPE, OPER)\
	}

#define SV_OP_METHOD_ALLOPER_CUSTOM(TYPE_A, TYPE_B, RES_TYPE, METHOD) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, +) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, +=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, -) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, -=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, *) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, *=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, /) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, /=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, ==) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, !=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, >) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, >=) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, <) \
	METHOD(TYPE_A, TYPE_B, RES_TYPE, <=)

#define SV_OP_METHOD_ALLOPER_NOOPER(TYPE_A, TYPE_B, RES_TYPE) \
	SV_OP_METHOD_ALLOPER_CUSTOM(TYPE_A, TYPE_B, RES_TYPE, SV_OP_SINGLE_OPER)

#define SV_OP_METHOD_ALLOPER(TYPE_A, TYPE_B, RES_TYPE) \
	SV_OP_METHOD_ALLOPER_CUSTOM(TYPE_A, TYPE_B, RES_TYPE, SV_OP_METHOD)
// END DEFINITIONS


	ScriptValue::~ScriptValue()
	{
		std::cout << "REMOVED " << this << std::endl;
	}

	// SCRIPT CLASS VALUE

	ScriptClassValue::ScriptClassValue(ScriptClassInstance *v)
		:val(v),
		type(new ScriptClassType(v->getClass()))
	{
	}

	ScriptClassValue::ScriptClassValue(ScriptClassInstance *v, ScriptClass &sclass)
		: val(v),
		type(new ScriptClassType(sclass))
	{
	}
}