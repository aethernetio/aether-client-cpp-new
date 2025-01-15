cmake_minimum_required(VERSION 3.16.0)

list(APPEND unit_tests_srcs "libhydrogen_unit_tests.cpp")

list(APPEND unit_tests_runner_srcs "unit_tests_runner.cpp")

if(NOT CM_PLATFORM)
    project(libhydrogen_tests C CXX)

    add_executable(${PROJECT_NAME}
                   ${unit_tests_srcs}
                   ${unit_tests_runner_srcs})

    target_include_directories(${PROJECT_NAME} PRIVATE ../../)

    target_link_libraries(${PROJECT_NAME}  PRIVATE hydrogen unity)
    
    add_test(NAME ${PROJECT_NAME}  COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
endif()
