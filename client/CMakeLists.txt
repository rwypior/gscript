cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GScriptClient")


add_executable(gscriptClient)
set_property(TARGET gscriptClient PROPERTY CXX_STANDARD 17)
set_target_properties(gscriptClient PROPERTIES OUTPUT_NAME "gscript")

add_dependencies(gscriptClient gscript)

target_sources(gscriptClient PRIVATE "main.cpp")

if(NOT DEFINED BUILD_SHARED_LIBS OR NOT ${BUILD_SHARED_LIBS})
    target_compile_definitions(gscriptClient PRIVATE BUILD_NOLIB)
endif()

target_link_libraries(gscriptClient PRIVATE gscript)

# Copy DLLs to exe directory

add_custom_command(TARGET gscriptClient POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:gscriptClient> $<TARGET_RUNTIME_DLLS:gscriptClient>
  COMMAND_EXPAND_LISTS
)

# Install

install(
        TARGETS gscriptClient EXPORT gscriptTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)