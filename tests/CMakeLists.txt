cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GScriptTest")

add_executable(gscriptTest)
set_property(TARGET gscriptTest PROPERTY CXX_STANDARD 17)

target_sources(gscriptTest PRIVATE "common.h" "main.cpp" "parser/commentTest.cpp")

# We're compiling against obj files - don't use DLL macros
target_compile_definitions(gscriptTest PRIVATE BUILD_NOLIB)

target_compile_definitions(gscriptTest
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

find_package(Catch2 3.6.0 REQUIRED)

add_library(gscriptobj STATIC $<TARGET_OBJECTS:gscript>)

target_include_directories(gscriptTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_include_directories(gscriptTest PRIVATE "${CMAKE_SOURCE_DIR}/include/gscript")
target_include_directories(gscriptTest PRIVATE ".")

target_link_libraries(gscriptTest PRIVATE gscriptobj)
target_link_libraries(gscriptTest PRIVATE Catch2::Catch2WithMain)

add_subdirectory("compiler")
add_subdirectory("extension")
add_subdirectory("integration")
add_subdirectory("parser")
add_subdirectory("runtime")
add_subdirectory("util")

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(gscriptTest)
