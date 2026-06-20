cmake_minimum_required(VERSION 3.19)
project(QtProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_executable(MyApp main.cpp)

target_link_libraries(MyApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)