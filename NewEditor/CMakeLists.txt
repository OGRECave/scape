project(Editor)

cmake_minimum_required(VERSION 2.8.11)


if(WIN32)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
        set(OGRE_SAMPLES_INCLUDEPATH
                $ENV{OGRE_HOME}/Samples/include
        )
endif(WIN32)

if(UNIX)
        if(EXISTS "/usr/local/lib/OGRE/cmake")

          set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
          set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

        elseif(EXISTS "/usr/lib/OGRE/cmake")

          set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
          set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

        else ()
          message(SEND_ERROR "Failed to find module path.")
        endif(EXISTS "/usr/local/lib/OGRE")
endif(UNIX)


if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    MESSAGE("DEBUG COMPILATION")
    ADD_DEFINITIONS("-DDEBUG")
else()
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX})

enable_language(CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${SCAPE_SOURCE_DIR})

set(SRC_FILES
./main.cpp
./mainwindow.cpp
./ogrewidget.cpp
)

set(HEADER_FILES
./mainwindow.h
./ogrewidget.h
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

if(UNIX)
        if(EXISTS "/usr/share/OGRE/cmake/modules/")

          set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules/;${CMAKE_MODULE_PATH}")

        elseif(EXISTS "/usr/lib/OGRE/cmake")

          set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
          set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

        else ()
          message(SEND_ERROR "Failed to find module path.")
        endif(EXISTS "/usr/share/OGRE/cmake/modules/")
endif(UNIX)


find_package(OGRE REQUIRED)

include_directories(${OGRE_INCLUDE_DIRS})

if(NOT ((${OGRE_VERSION} VERSION_EQUAL "1.8.0") OR (${OGRE_VERSION} VERSION_GREATER "1.8.0")))
    message(SEND_ERROR "You need Ogre 1.8.0 or later to build this. You can get it from sinbad's repository here: https://bitbucket.org/sinbad/ogre")
endif()

find_package(OIS REQUIRED)


if(NOT OIS_FOUND)
        message(SEND_ERROR "Failed to find OIS.")
endif()

# Find Boost
if(NOT OGRE_BUILD_PLATFORM_IPHONE)
        if(WIN32 OR APPLE)
                set(Boost_USE_STATIC_LIBS TRUE)
        else()
                # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
                set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
        endif()
        if(MINGW)
                # this is probably a bug in CMake: the boost find module tries to look for
                # boost libraries with name libboost_*, but CMake already prefixes library
                # search names with "lib". This is the workaround.
                set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
        endif()
        set(Boost_ADDITIONAL_VERSIONS "1.50" "1.51" "1.51.0" "1.52" "1.52.0" "1.53" "1.53.0" "1.54" "1.54.0" "1.55" "1.55.0")
        # Components that need linking (NB does not include header-only components like bind)
        set(OGRE_BOOST_COMPONENTS thread system REQUIRED)
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
        if (NOT Boost_FOUND)
                # Try again with the other type of libs
                set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
                find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
        endif()
        find_package(Boost QUIET)

        # Set up referencing of Boost
        include_directories(${Boost_INCLUDE_DIR})
        add_definitions(-DBOOST_ALL_NO_LIB)
        set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()

#Find Qt
set(Qt5Core_DIR "$ENV{QTDIR}/lib/CMake/Qt5Core/")
set(Qt5Widgets_DIR "$ENV{QTDIR}/lib/CMake/Qt5Widgets/")
set(Qt5Gui_DIR "$ENV{QTDIR}/lib/CMake/Qt5GUI/")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

if(UNIX)
    set(Qt5X11Extras_DIR "$ENV{QTDIR}/lib/CMake/QtX11Extras/")
    find_package(Qt5X11Extras REQUIRED)
endif()

# if we don't include this CMake will not include ui headers properly:
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(UI_SRCS ${UI_FILES})

qt5_add_resources(RESOURCE_SRCS ./res/editor.qrc)

if(WIN32)
        file(GLOB RES ./res/*.rc)
        set(RESOURCE_SRCS ${RESOURCE_SRCS} ${RES})
endif(WIN32)

if(WIN32)
        add_executable(Editor WIN32
        ${HEADER_FILES}
        ${SRC_FILES}
        ${RESOURCE_SRCS}
        ${UI_SRCS}
        )
else()
        add_executable(Editor
        ${HEADER_FILES}
        ${SRC_FILES}
        ${RESOURCE_SRCS}
        ${UI_SRCS}
        )
endif()

if(UNIX)
    qt5_use_modules(Editor Core Gui Widgets X11Extras)
    target_link_libraries(Editor
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::X11Extras
    Engine
    )
elseif(WIN32)
    set_target_properties(Editor PROPERTIES DEBUG_POSTFIX _d)
    set_property(TARGET Editor PROPERTY WIN32_EXECUTABLE 0)

    qt5_use_modules(Editor Core Gui Widgets)
    target_link_libraries(Editor
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Engine
    )
endif()

# Installation
if(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/windows.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/unix.cmake)
endif()
