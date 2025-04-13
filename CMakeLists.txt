cmake_minimum_required(VERSION 3.10)
project(NodeBasedImageProcessor)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES src/main.cpp src/other_module.cpp)  

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
