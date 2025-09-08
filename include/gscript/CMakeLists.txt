target_sources(gscript PRIVATE
	"classKit.hpp" "compileException.hpp" "debug.hpp" "defs.hpp" "EntityPath.hpp" "extensionKit.hpp" "functionKit.hpp"
	"IteratorRange.hpp" "lib.hpp" "path.hpp" "runtimeException.hpp" "script.hpp" "StringUtils.hpp" "type.hpp" "util.hpp"
	"bitfield.hpp" "logger.hpp"
)
add_subdirectory("compiler")
add_subdirectory("extension")
add_subdirectory("factory")
add_subdirectory("framework")
add_subdirectory("parser")
add_subdirectory("runtime")
