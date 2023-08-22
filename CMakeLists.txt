cmake_minimum_required(VERSION 3.21)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

set(VCPKG_PATH YOUR_PATH)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH} CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE YOUR_PATH)
set(CMAKE_PREFIX_PATH YOUR_PATH)


find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
        main.cpp
        mywindow.cpp
        )

set(HEADERS
        mywindow.h
        )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
