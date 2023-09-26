cmake_minimum_required(VERSION 3.11)

project(dbeditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        dbview.cpp
        dbview.h
)

if(ANDROID)
    add_library(dbeditor SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(dbeditor
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(dbeditor PRIVATE Qt5::Widgets Qt5::Sql)
