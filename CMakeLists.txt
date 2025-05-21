cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_DEBUG_POSTFIX "d")

project ("GScript")
add_library(gscript)
add_library(gscript::gscript ALIAS gscript)

set_property(TARGET gscript PROPERTY CXX_STANDARD 17)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(gscript PRIVATE BUILD_SCRIPT_DLL)
else()
    target_compile_definitions(gscript PRIVATE BUILD_NOLIB)
endif()

target_include_directories (gscript PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/gscript>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_subdirectory ("src")
add_subdirectory ("include/gscript")
add_subdirectory ("client")

if(${BUILD_TESTING})
    message("Building unit tests")

    include(CTest)
    add_subdirectory ("tests")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(ConfigPackageLocation lib/cmake/gscript)

configure_package_config_file(
        cmake/gscriptConfig.cmake.in
	    "${CMAKE_CURRENT_BINARY_DIR}/gscript/gscriptConfig.cmake"
	    INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/gscript"
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/gscript/gscriptConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY AnyNewerVersion
)

install(
        TARGETS gscript EXPORT gscriptTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING 
        PATTERN "*.h"
        PATTERN "*.hpp"
)

install(EXPORT gscriptTargets
        FILE gscriptTargets.cmake
        NAMESPACE gscript::
        DESTINATION ${ConfigPackageLocation}
)

install(FILES 
            "${CMAKE_CURRENT_BINARY_DIR}/gscript/gscriptConfig.cmake" 
            "${CMAKE_CURRENT_BINARY_DIR}/gscript/gscriptConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation}
)

export(PACKAGE gscript)
