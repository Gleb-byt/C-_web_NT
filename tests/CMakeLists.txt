add_executable(json_tests data_parsers_tests.cpp)

target_link_libraries(
    json_tests

    PRIVATE

    json_lib

)

