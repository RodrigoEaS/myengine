cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME engine)
project(${PROJECT_NAME} VERSION 1.0.0)

#COPY RESOURCES
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

#ADD SOURCES FILES
file(GLOB_RECURSE SRC_FILES src/*.cpp)

#FIND OPENGL
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

#ADD LIBRARIES (subdirectories)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/json-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/stb)

#CREATE THE EXECUTABLE
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

#LINK LIBRARIES
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC json-cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC stb)

#ADD HEADERS FILES
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)