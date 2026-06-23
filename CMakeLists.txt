cmake_minimum_required(VERSION 3.19)
project(QtProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

qt_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/audioplayer.cpp
)

set(HEADERS
    include/mainwindow.h
    include/audioplayer.h
)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)