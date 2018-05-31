project(qtEditor)

cmake_minimum_required(VERSION 2.8.11)

if(WIN32)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
        set(OGRE_SAMPLES_INCLUDEPATH
                $ENV{OGRE_HOME}/Samples/include
        )
endif(WIN32)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    MESSAGE("DEBUG COMPILATION")
    ADD_DEFINITIONS("-DDEBUG")
else()
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

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

ExternalProject_Get_Property(QtSolutions_PropertyBrowser install_dir)
include_directories(${install_dir}/src/QtSolutions_PropertyBrowser/src)

set(SRC_FILES
./main.cpp
./mainwindow.cpp
./ogrewidget.cpp
./mainwindow_actions.cpp
./aboutdialog.cpp
./propertieswidget.cpp
./presetswidget.cpp
./complexproperties.cpp
)

set(HEADER_FILES
./mainwindow.h
./ogrewidget.h
./aboutdialog.h
./propertieswidget.h
./presetswidget.h
./complexproperties.h
./stdredirector.h
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

find_package(OGRE REQUIRED)

include_directories(${OGRE_INCLUDE_DIRS})

if(NOT ((${OGRE_VERSION} VERSION_EQUAL "1.8.0") OR (${OGRE_VERSION} VERSION_GREATER "1.8.0")))
    message(SEND_ERROR "You need Ogre 1.8.0 or later to build this. You can get it from sinbad's repository here: https://bitbucket.org/sinbad/ogre")
endif()

find_package(Qt5Widgets REQUIRED)

# if we don't include this CMake will not include ui headers properly:
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_wrap_ui(UI_SRCS ${UI_FILES})

qt5_add_resources(RESOURCE_SRCS ./res/Editor.qrc)

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
    )
endif()

add_dependencies(qtEditor QtSolutions_PropertyBrowser)

if(UNIX AND NOT APPLE)
        TARGET_LINK_LIBRARIES(qtEditor
         debug ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser_d.so
         optimized ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser.so)
elseif(APPLE)
        TARGET_LINK_LIBRARIES(qtEditor
         debug ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser_d.so
         optimized ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser.so)
elseif(WIN32)
    if(MSVC_IDE)
        TARGET_LINK_LIBRARIES(qtEditor
         debug ${CMAKE_BINARY_DIR}/Dependencies/lib/Debug/QtSolutions_PropertyBrowser_d.lib
         optimized ${CMAKE_BINARY_DIR}/Dependencies/lib/Release/QtSolutions_PropertyBrowser.lib)
    else()
        TARGET_LINK_LIBRARIES(qtEditor
            debug ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser_d.lib
            optimized ${CMAKE_BINARY_DIR}/Dependencies/lib/QtSolutions_PropertyBrowser.lib)
    endif(MSVC_IDE)
endif(UNIX AND NOT APPLE)

# Installation
if(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/windows.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/unix.cmake)
endif()
