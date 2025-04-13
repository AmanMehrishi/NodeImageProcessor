cmake_minimum_required(VERSION 3.10)
project(NodeBasedImageProcessor)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "D:/opencv/build/x64/vc16/lib")

include_directories(include)

set(SOURCES
    src/main.cpp
    src/ImageInputNode.cpp
    src/EdgeDetectionNode.cpp
    src/OutputNode.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()
