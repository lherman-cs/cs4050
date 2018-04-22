
cmake_minimum_required(VERSION 3.1.0)

project(homework-3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Ofast -frename-registers -funroll-loops -fno-signed-zeros -fno-trapping-math") 

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SRCS *.cc)
add_executable(homework-3 ${SRCS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(homework-3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
