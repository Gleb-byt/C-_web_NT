cmake_minimum_required(VERSION 3.20)
project(web_server CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(web_server src/main.cpp)

target_link_libraries(web_server PRIVATE Boost::boost)

if(WIN32)
    target_link_libraries(web_server PRIVATE ws2_32 mswsock)
endif()