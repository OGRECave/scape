project(Editor)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR})

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

#Find Qt
set(Qt5Core_DIR "$ENV{QTDIR}/lib/CMake/Qt5Core/")
set(Qt5Widgets_DIR "$ENV{QTDIR}/lib/CMake/Qt5Widgets/")
set(Qt5Gui_DIR "$ENV{QTDIR}/lib/CMake/Qt5GUI/")

#find_package(Qt REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

#X11 is required to operate on Linux windows
if(UNIX)
    set(Qt5X11Extras_DIR "$ENV{QTDIR}/lib/CMake/QtX11Extras/")
    find_package(Qt5X11Extras REQUIRED)
endif()

# if we don't include this CMake will not include ui headers properly:
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(UI_SRCS ${UI_FILES})

#qt5_add_resources(RESOURCE_SRCS ./resource.qrc)

##if(WIN32)
#   file(GLOB RES ./*.rc)
#   set(RESOURCE_SRCS ${RESOURCE_SRCS} ${RES})
##endif(WIN32)

if(WIN32)
    add_executable(Editor WIN32
    ${HEADER_FILES}
    ${SRC_FILES}
#   ${RESOURCE_SRCS}
    ${UI_SRCS}
    )
else()
    add_executable(Editor
    ${HEADER_FILES}
    ${SRC_FILES}
#   ${RESOURCE_SRCS}
    ${UI_SRCS}
    )
endif()

if(UNIX)
    qt5_use_modules(Editor Core Gui Widgets X11Extras)
    target_link_libraries(Editor
    Engine
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    Qt5::X11Extras
    )
elseif(WIN32)
    qt5_use_modules(Editor Core Gui Widgets)
    target_link_libraries(Editor
    Engine
    ${Boost_SYSTEM_LIBRARY}
    ${OGRE_LIBRARIES}
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets
    )
endif()

#install(TARGETS Editor DESTINATION ${CMAKE_INSTALL_PREFIX})

GET_FILENAME_COMPONENT(QT_DLL_DIR ${QT_QMAKE_EXECUTABLE_FINDQT} PATH)

if (DEFINED QT_QMAKE_EXECUTABLE_FINDQT)
    SET (QTLIBLIST 
    Qt5GUI 
    Qt5Core 
    Qt5Widgets
    )
    FOREACH(qtlib ${QTLIBLIST})
      IF (WIN32)
        INSTALL(FILES ${QT_DLL_DIR}/${qtlib}.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
      ENDIF (WIN32)
    ENDFOREACH(qtlib)
endif(DEFINED QT_QMAKE_EXECUTABLE_FINDQT)

IF (WIN32)
    SET (QTPLATFORMSLIST 
        qwindows)
    FOREACH(platform ${QTPLATFORMSLIST})
        INSTALL(FILES $ENV{QTDIR}/plugins/platforms/${platform}.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/platforms/)
    ENDFOREACH(platform)
ENDIF (WIN32)

SET (OGRELIBLIST 
    OgreMain
    OIS
    cg
    RenderSystem_GL
    Plugin_OctreeSceneManager
    Plugin_BSPSceneManager
    Plugin_CgProgramManager
    Plugin_ParticleFX
    Plugin_PCZSceneManager
    Plugin_OctreeZone
    OgrePaging
    OgreProperty
    OgreTerrain
    OgreRTShaderSystem
    OgreVolume
    OgreOverlay)
FOREACH(ogrelib ${OGRELIBLIST})
  IF (WIN32)
    INSTALL(FILES $ENV{OGRE_HOME}/bin/Release/${ogrelib}.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
  ENDIF (WIN32)
ENDFOREACH(ogrelib)

install(FILES
        plugins.cfg
        resources.cfg
        DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES
        SdkTrays.zip
        OgreCore.zip
        DESTINATION ${CMAKE_INSTALL_PREFIX})
