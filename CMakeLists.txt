cmake_minimum_required(VERSION 3.16)
project(ImageViewer LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    main/ImageViewer.ui
    main/ImageViewer.h
    main/ImageViewer.cpp
    Widgets/MyGraphicsView.h
    Widgets/MyGraphicsView.cpp
    Widgets/MyLabel.h
    Widgets/MyLabel.cpp
    manager/thumbnailManager.h
    manager/thumbnailManager.cpp
    manager/imageManager.h
    manager/imageManager.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        main
        Widgets
        manager
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE FALSE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

