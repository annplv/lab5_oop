cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS} ../include)

add_executable(run_tests
    test_list.cpp
)

target_link_libraries(run_tests
    ${GTEST_LIBRARIES}
    pthread
)

