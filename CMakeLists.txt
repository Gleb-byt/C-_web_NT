cmake_minimum_required(VERSION 3.20)
project(web_server CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(server_lib
    src/api/api_client.hpp
    src/api/json_helpers.hpp

    src/api/api_client.cpp
    src/api/json_helpers.cpp
)

target_link_libraries(server_lib PUBLIC 
    Boost::boost
    nlohmann_json::nlohmann_json
)

target_include_directories(server_lib PUBLIC src)

add_executable(web_server src/main.cpp)

target_link_libraries(web_server PRIVATE server_lib)

if(WIN32)
    target_link_libraries(web_server PRIVATE ws2_32 mswsock)
endif()