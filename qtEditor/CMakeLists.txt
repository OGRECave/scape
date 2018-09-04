project(qtEditor)

cmake_minimum_required(VERSION 2.8.11)

if(WIN32)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
        set(OGRE_SAMPLES_INCLUDEPATH
                $ENV{OGRE_HOME}/Samples/include
        )
endif(WIN32)

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX})
# Force MSVC_IDE to not put the executable in Debug/Release directories!
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_INSTALL_PREFIX})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_INSTALL_PREFIX})

enable_language(CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${SCAPE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/Dependencies/QtPropertyBrowser/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/QtPropertyBrowser QtPropertyBrowser)

set(SRC_FILES
./main.cpp
./mainwindow.cpp
./ogrewidget.cpp
./mainwindow_actions.cpp
./aboutdialog.cpp
./propertieswidget.cpp
./presetswidget.cpp
./complexproperties.cpp
./ImageFileDialog.cpp
)

set(HEADER_FILES
./mainwindow.h
./ogrewidget.h
./aboutdialog.h
./propertieswidget.h
./presetswidget.h
./complexproperties.h
./ImageFileDialog.h
)

file(GLOB UI_FILES ./*.ui)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

#Find Ogre
if(WIN32)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
        set(OGRE_SAMPLES_INCLUDEPATH
                $ENV{OGRE_HOME}/Samples/include
        )
endif(WIN32)

find_package(OGRE 1.11 REQUIRED)
find_package(Qt5Widgets REQUIRED)

# if we don't include this CMake will not include ui headers properly:
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_wrap_ui(UI_SRCS ${UI_FILES})

qt5_add_resources(RESOURCE_SRCS ./res/editor.qrc)

include_directories(${Qt5Widgets_INCLUDE_DIRS})

if(WIN32)

        file(GLOB RES ./res/*.rc)
        set(RESOURCE_SRCS ${RESOURCE_SRCS} ${RES})
endif(WIN32)

if(WIN32)
        add_executable(qtEditor WIN32
        ${HEADER_FILES}
        ${SRC_FILES}
        ${RESOURCE_SRCS}
        ${UI_SRCS}
        )
else()
        add_executable(qtEditor
        ${HEADER_FILES}
        ${SRC_FILES}
        ${RESOURCE_SRCS}
        ${UI_SRCS}
        )
endif()

if(UNIX)
    qt5_use_modules(qtEditor Core Gui Widgets X11Extras)
    target_link_libraries(qtEditor
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::X11Extras
    Engine
    QtPropertyBrowser
    )
elseif(WIN32)
    qt5_use_modules(qtEditor Core Gui Widgets)
    set_target_properties(qtEditor PROPERTIES DEBUG_POSTFIX _d)
    set_target_properties(qtEditor PROPERTIES COMPILE_DEFINITIONS QT_QTPROPERTYBROWSER_IMPORT)
    set_property(TARGET qtEditor PROPERTY WIN32_EXECUTABLE 0)

    target_link_libraries(qtEditor
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Engine
    QtPropertyBrowser
    )
endif()

# Installation
if(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/windows.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/unix.cmake)
endif()
