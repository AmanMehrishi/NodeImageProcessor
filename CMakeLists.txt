cmake_minimum_required(VERSION 3.19)
project(NodeBasedImageProcessor)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/permissive- /Zc:__cplusplus)
endif()

set(CMAKE_DISABLE_FIND_PACKAGE_Vulkan TRUE)

set(OpenCV_DIR "D:/opencv/build/x64/vc16/lib")

set(Qt6_DIR "D:/Qt2/6.10.0/msvc2022_64/lib/cmake/Qt6")

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)

qt_standard_project_setup()

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/ImageInputNode.cpp
    ${CMAKE_SOURCE_DIR}/src/BrightnessContrastNode.cpp
    ${CMAKE_SOURCE_DIR}/src/EdgeDetectionNode.cpp
    ${CMAKE_SOURCE_DIR}/src/OutputNode.cpp
    ${CMAKE_SOURCE_DIR}/src/NodeGraph.cpp
    ${CMAKE_SOURCE_DIR}/include/MainWindow.h
)

qt_add_executable(${PROJECT_NAME} WIN32 ${SOURCES})



target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui ${OpenCV_LIBS})
