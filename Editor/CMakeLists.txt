PROJECT( wxEditor )

#set(wxWidgets_LIB_DIR "C:/wxWidgets-2.8.3/lib/vc_lib")

#FIND_PACKAGE(wxWidgets COMPONENTS core base aui adv propgrid REQUIRED)
#include( "${wxWidgets_USE_FILE}" )

#include_directories( "${wxEditor_SOURCE_DIR}" )
include_directories(${CMAKE_SOURCE_DIR}/Engine
   ${CMAKE_SOURCE_DIR}/Engine/EngineCore
   ${CMAKE_SOURCE_DIR}/Engine/GPU2DOperation
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldBrush
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldBuffer
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldFileCodecs
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldGeom
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldOperation
   ${CMAKE_SOURCE_DIR}/Engine/HeightfieldSerializer
   ${CMAKE_SOURCE_DIR}/Engine/Input
   ${CMAKE_SOURCE_DIR}/Engine/PCH
   ${CMAKE_SOURCE_DIR}/Engine/Tinyxml
   ${CMAKE_SOURCE_DIR}/Engine/Utils
   ${CMAKE_SOURCE_DIR}/Engine/Wintab
)

FILE(GLOB Application_SCRS "./Application/*.cpp")
FILE(GLOB Controls_SCRS "./Controls/*.cpp")
FILE(GLOB PCH_SCRS "./PCH/*.cpp")
FILE(GLOB Utils_SCRS "./Utils/*.cpp")
FILE(GLOB Windows_SCRS "./Windows/*.cpp")

FILE(GLOB Application_HDRS "./Application/*.h")
FILE(GLOB Controls_HDRS "./Controls/*.h")
FILE(GLOB PCH_HDRS "./PCH/*.h")
FILE(GLOB Utils_HDRS "./Utils/*.h")
FILE(GLOB Windows_HDRS "./Windows/*.h")

INCLUDE_DIRECTORIES (
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/Application
   ${CMAKE_CURRENT_SOURCE_DIR}/Controls
   ${CMAKE_CURRENT_SOURCE_DIR}/PCH
   ${CMAKE_CURRENT_SOURCE_DIR}/Utils
   ${CMAKE_CURRENT_SOURCE_DIR}/Windows
)

set (HEADER_FILES
   ${Application_HDRS} 
   ${Controls_HDRS} 
   ${PCH_HDRS} 
   ${Utils_HDRS} 
   ${Windows_HDRS} 
)

set (SOURCE_FILES
   ${Application_SCRS} 
   ${Controls_SCRS} 
   ${PCH_SCRS} 
   ${Utils_SCRS} 
   ${Windows_SCRS} 
)

if(WIN32)
    # add icon
    set (SOURCE_FILES ${SOURCE_FILES} ${wxEditor_SOURCE_DIR}/Resources/Editor.rc)
endif()

#ADD_EXECUTABLE(wxEditor WIN32 ${HEADER_FILES} ${SOURCE_FILES} )
#TARGET_LINK_LIBRARIES( wxEditor ${wxWidgets_LIBRARIES} )
#TARGET_LINK_LIBRARIES( wxEditor ${OGRE_LIBRARIES} )
#TARGET_LINK_LIBRARIES( wxEditor Engine )
