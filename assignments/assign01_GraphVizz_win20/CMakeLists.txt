cmake_minimum_required(VERSION 4.0)
project(graph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# specify the qt directory
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.9.2")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(graph src/main.cpp
        src/SimpleGraph.cpp
        src/SimpleGraph.h)
target_link_libraries(graph
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

