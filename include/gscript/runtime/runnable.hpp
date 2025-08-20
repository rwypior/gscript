//#ifndef _h_gscript_runnable
//#define _h_gscript_runnable
//
//#include "callable.hpp"
//#include "scope.hpp"
////#include "prototype.hpp"
//
//#include <vector>
//#include <memory>
//
//namespace gscript
//{
//	class ScriptValue;
//	class ParserBlockBody;
//
//	// ScriptRunnable is any actively-executable part of code that runs concrete code
//	// This includes control statements, return statements, other callables or statements
//
//	class ScriptRunnable : public ScriptScope//, public PrototypeResolver
//	{
//	public:
//		SCRIPT_API ScriptRunnable(ScriptScope *parentScope, std::vector<std::shared_ptr<ScriptCallable>>&& statements);
//
//		SCRIPT_API virtual ScriptValue *run();
//		//SCRIPT_API void setup(const ParserBlockBody &pbody);
//
//		//virtual void setupPrototypes(ScriptScope& scope) override;
//
//	private:
//		std::vector<std::shared_ptr<ScriptCallable>> statements;
//	};
//}
//
//#endif