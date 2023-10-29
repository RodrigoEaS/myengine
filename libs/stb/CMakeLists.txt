cmake_minimum_required(VERSION 3.11)

add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)