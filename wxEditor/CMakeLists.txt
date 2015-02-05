PROJECT( wxEditor )

set(wxWidgets_ROOT_DIR C:/Programming/scape/Dependencies/wxWidgets-2.8.3)
set(wxWidgets_LIB_DIR C:/Programming/scape/Dependencies/wxWidgets-2.8.3/lib/vc_dll)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "debug")
    #SET(wxWidgets_USE_REL_AND_DBG ON)
    #set(wxWidgets_CONFIGURATION "mswd")
    ADD_DEFINITIONS("-DDEBUG")
else()
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

FIND_PACKAGE(wxWidgets COMPONENTS core base aui adv propgrid REQUIRED)
include( "${wxWidgets_USE_FILE}" )

include_directories( "${wxEditor_SOURCE_DIR}" )

set (HEADER_FILES
    MainFrame.h
    OgreditorViewUtils.h
    paths.h
    renderpanel.h
    wxhelperfunctions.h
    wxOgreditor.h
    )

set (SOURCE_FILES
    MainFrame.cpp
    OgreditorViewUtils.cpp
    renderpanel.cpp
    wxhelperfunctions.cpp
    paths.cpp
    wxOgreditor.cpp
    )


if(WIN32)
    # add icon
    set (SOURCE_FILES ${SOURCE_FILES} ${wxEditor_SOURCE_DIR}/res/wxEditor.rc)
endif()

link_directories(${OGRE_LIBRARY_DIRS})
ADD_EXECUTABLE(wxEditor WIN32 ${HEADER_FILES} ${SOURCE_FILES} )
set_target_properties(wxEditor PROPERTIES DEBUG_POSTFIX _d)
TARGET_LINK_LIBRARIES( wxEditor ${wxWidgets_LIBRARIES} )
TARGET_LINK_LIBRARIES( wxEditor ${OGRE_LIBRARIES} Engine)
