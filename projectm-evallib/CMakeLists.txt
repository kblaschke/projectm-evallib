
add_library(projectMEvalLib STATIC
        CodePreprocessor.cpp
        CodePreprocessor.hpp
        )

target_include_directories(projectMEvalLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${PROJECTM_INCLUDE_DIR}>
        )

set_target_properties(projectMEvalLib PROPERTIES
        OUTPUT_NAME $<IF:$<PLATFORM_ID:Windows>,libprojectM-EvalLib,projectM-EvalLib>
        EXPORT_NAME EvalLib
        )

install(TARGETS projectMEvalLib
        EXPORT projectMEvalLibTargets
        ARCHIVE DESTINATION "${PROJECTM_LIB_DIR}"
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${PROJECTM_INCLUDE_DIR}
        FILES_MATCHING PATTERN *.hpp
        )

# For use from an installed package (system install, vcpkg, homebrew etc.)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/projectMEvalLibConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(projectMEvalLibConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/projectMEvalLibConfig.cmake"
        INSTALL_DESTINATION "${PROJECTM_LIB_DIR}/cmake/projectMEvalLib"
        PATH_VARS PROJECTM_LIB_DIR PROJECTM_INCLUDE_DIR
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/projectMEvalLibConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/projectMEvalLibConfig.cmake"
        DESTINATION "${PROJECTM_LIB_DIR}/cmake/projectMEvalLib"
        COMPONENT Devel
        )

install(EXPORT projectMEvalLibTargets
        FILE projectMEvalLibTargets.cmake
        NAMESPACE libprojectM::
        DESTINATION "${PROJECTM_LIB_DIR}/cmake/projectMEvalLib"
        COMPONENT Devel
        )
