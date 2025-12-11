cmake_minimum_required(VERSION 3.16)
project(LR3 CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")

add_executable(tests
    test/main.cpp
    test/treeAVL_test.cpp
    test/array_test.cpp
    test/stack_test.cpp
    test/queue_test.cpp
    test/doubleList_test.cpp
    test/singleList_test.cpp
    test/hashTable_test.cpp
)

target_link_libraries(tests gtest gtest_main)
target_include_directories(tests PRIVATE . test)

add_custom_target(coverage
    COMMAND mkdir -p coverage
    COMMAND rm -f *.gcda *.gcno CMakeFiles/tests.dir/test/*.gcda CMakeFiles/tests.dir/test/*.gcno
    COMMAND ./tests
    COMMAND lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
    COMMAND lcov --remove coverage.info '/usr/*' '*/test/*' '*/build/*' --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage --ignore-errors source
    COMMAND echo "✓ Отчет готов: file://$(pwd)/coverage/index.html"
)
