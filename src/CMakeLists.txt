target_sources(gscript PRIVATE
	"defs.cpp" "EntityPath.cpp" "script.cpp" "type.cpp" "util.cpp"
	"logger.cpp" "parseexception.cpp")
add_subdirectory("compiler")
add_subdirectory("extension")
add_subdirectory("factory")
add_subdirectory("framework")
add_subdirectory("parser")
add_subdirectory("runtime")
