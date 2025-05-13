cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GScriptTest")

add_executable(gscriptTest)
set_property(TARGET gscriptTest PROPERTY CXX_STANDARD 17)

find_package(Catch2 3.6.0 REQUIRED)

target_include_directories(gscriptTest PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_include_directories(gscriptTest PRIVATE "include")

target_link_libraries(gscriptTest PRIVATE gscript)
target_link_libraries(gscriptTest PRIVATE Catch2::Catch2WithMain)

add_subdirectory ("src")
add_subdirectory ("include")

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(gscriptTest)
