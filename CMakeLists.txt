cmake_minimum_required(VERSION 3.27)

project(ambience VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

qt_add_executable(ambience main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Light.cpp
        Light.h
        light.cpp
        light.h
        lifxpacket.cpp
        lifxpacket.h
        lifxlan.cpp
        lifxlan.h
        lightmodel.cpp
        lightmodel.h
        scanwindow.cpp
        scanwindow.h
        scanwindow.ui
)
target_link_libraries(ambience
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)