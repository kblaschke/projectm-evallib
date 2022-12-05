find_package(GTest 1.10 REQUIRED NO_MODULE)

add_executable(projectM-EvalLib-Test
    CodePreprocessorTest.cpp
    )

target_compile_definitions(projectM-EvalLib-Test
    PRIVATE
    PROJECTM_EVALLIB_TEST_DATA_DIR="${CMAKE_CURRENT_LIST_DIR}/data"
    )

# Test includes a header file from libprojectM with its full path in the source dir.
target_include_directories(projectM-EvalLib-Test
    PRIVATE
    "${CMAKE_SOURCE_DIR}"
    )

target_link_libraries(projectM-EvalLib-Test
    PRIVATE
    projectMEvalLib
    GTest::gtest
    GTest::gtest_main
    )

add_test(NAME projectM-EvalLib-Test COMMAND projectM-EvalLib-Test)
