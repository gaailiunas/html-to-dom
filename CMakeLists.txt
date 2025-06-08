cmake_minimum_required(VERSION 3.10)
project(html-to-dom)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED) 

add_executable(html-to-dom src/main.c)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(html-to-dom ${CURL_LIBRARIES})