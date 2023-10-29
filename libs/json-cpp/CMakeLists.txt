cmake_minimum_required(VERSION 3.11)

add_library(json-cpp STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/json.cpp)
target_include_directories(json-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)