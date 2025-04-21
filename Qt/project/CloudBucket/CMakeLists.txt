cmake_minimum_required(VERSION 3.5)

project(CloudBucket LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 确保在构建过程中找到 Qt5 的 Widgets 和 Network 组件
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

if(ANDROID)
  add_library(CloudBucket SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(CloudBucket
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    listupdater.h
    listupdater.cpp
  )
endif()

# 链接 Qt5 的 Widgets 和 Network 模块
target_link_libraries(CloudBucket PRIVATE Qt5::Widgets Qt5::Network)
