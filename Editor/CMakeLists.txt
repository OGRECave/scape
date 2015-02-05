PROJECT( WXEditor )

#${CMAKE_SOURCE_DIR}/Dependencies/wxWidgets-2.8.3
add_definitions(-DWXUSINGDLL)

include_directories( "${WXEditor_SOURCE_DIR}" )
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/wxWidgets-2.8.3/include/msvc)
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/wxWidgets-2.8.3/include)
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/wxWidgets-2.8.3/contrib/include)

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

foreach(src_file ${CPP_FILESa})
   if (${src_file} MATCHES "stdafx.cpp$")
       set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
   else()
       get_filename_component(basename ${src_file} NAME)
      set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h -DDEBUG_CPP_NAME=\"\\\"${basename}\"\\\"")
   endif()
endforeach()


if(WIN32)
    # add icon
    set (SOURCE_FILES ${SOURCE_FILES} ${WXEditor_SOURCE_DIR}/Resources/Editor.rc)
endif()

ADD_EXECUTABLE(WXEditor WIN32 ${HEADER_FILES} ${SOURCE_FILES} )

set_target_properties(WXEditor PROPERTIES 
COMPILE_DEFINITIONS __WXMSW__
)

#TARGET_LINK_LIBRARIES( WXEditor ${wxWidgets_LIBRARIES} )
TARGET_LINK_LIBRARIES( WXEditor ${OGRE_LIBRARIES} )
TARGET_LINK_LIBRARIES( WXEditor Engine )
