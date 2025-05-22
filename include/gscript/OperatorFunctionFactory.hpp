#ifndef _h_gscript_operator_function_factory
#define _h_gscript_operator_function_factory

#include "defs.hpp"
#include "util.hpp"

#include <unordered_map>
#include <tuple>

namespace gscript
{
	class ScriptCallable;

	namespace OperatorFunctionFactory
	{
		typedef ScriptValue *(*OPERATOR_FUNCTION_T)(ScriptCallable *left, ScriptCallable *right);
		typedef std::tuple<VALUE_TYPE_T, VALUE_TYPE_T, OPERATOR_TYPE_T> FUNC_MAP_KEY_T;
		typedef std::unordered_map<FUNC_MAP_KEY_T, OPERATOR_FUNCTION_T, hash_tuple::hash<FUNC_MAP_KEY_T> > FUNCMAP_T;

		extern FUNCMAP_T funcmap;

		OPERATOR_FUNCTION_T getFunction(VALUE_TYPE_T left, VALUE_TYPE_T right, OPERATOR_TYPE_T oper);
	};
}

#endif