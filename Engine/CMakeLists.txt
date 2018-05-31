Project(Engine)

# Enables the use of Precompiled Headers in the project this macro is invoked in. Also adds the DEBUG_CPP_NAME to each .cpp file that specifies the name of that compilation unit. MSVC only.
#macro(SetupCompileFlagsWithPCH)
#    if (MSVC)
#        # Label StableHeaders.cpp to create the PCH file and mark all other .cpp files to use that PCH file.
#        # Add a #define DEBUG_CPP_NAME "this compilation unit name" to each compilation unit to aid in memory leak checking.
#        foreach(src_file ${CPP_FILES})
#            if (${src_file} MATCHES "stdafx.cpp$")
#                set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")        
#            else()
#                get_filename_component(basename ${src_file} NAME)
#                set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h -DDEBUG_CPP_NAME=\"\\\"${basename}\"\\\"")
#            endif()
#       endforeach()
#   endif()
#endmacro()

FILE(GLOB EngineCore_SRCS "./EngineCore/*.cpp")
FILE(GLOB GPU2DOperation_SRCS "./GPU2DOperation/*.cpp")
FILE(GLOB HeightfieldBrush_SRCS "./HeightfieldBrush/*.cpp")
FILE(GLOB HeightfieldBuffer_SRCS "./HeightfieldBuffer/*.cpp")
FILE(GLOB HeightfieldFileCodecs_SRCS "./HeightfieldFileCodecs/*.cpp")
FILE(GLOB HeightfieldGeom_SRCS "./HeightfieldGeom/*.cpp")
FILE(GLOB HeightfieldOperation_SRCS "./HeightfieldOperation/*.cpp")
FILE(GLOB HeightfieldSerializer_SRCS "./HeightfieldSerializer/*.cpp")
FILE(GLOB Input_SRCS "./Input/*.cpp")
FILE(GLOB Input_DEFS "./Input/*.def")
FILE(GLOB Tinyxml_SRCS "./Tinyxml/*.cpp")
FILE(GLOB Utils_SRCS "./Utils/*.cpp")
FILE(GLOB Wintab_SRCS "./Wintab/*.cpp")
FILE(GLOB PCH_SRCS "./PCH/*.cpp")


FILE(GLOB EngineCore_HDRS "./EngineCore/*.h")
FILE(GLOB GPU2DOperation_HDRS "./GPU2DOperation/*.h")
FILE(GLOB HeightfieldBrush_HDRS "./HeightfieldBrush/*.h")
FILE(GLOB HeightfieldBuffer_HDRS "./HeightfieldBuffer/*.h")
FILE(GLOB HeightfieldFileCodecs_HDRS "./HeightfieldFileCodecs/*.h")
FILE(GLOB HeightfieldGeom_HDRS "./HeightfieldGeom/*.h")
FILE(GLOB HeightfieldOperation_HDRS "./HeightfieldOperation/*.h")
FILE(GLOB HeightfieldSerializer_HDRS "./HeightfieldSerializer/*.h")
FILE(GLOB Input_HDRS "./Input/*.h")
FILE(GLOB Tinyxml_HDRS "./Tinyxml/*.h")
FILE(GLOB Utils_HDRS "./Utils/*.h")
FILE(GLOB Wintab_HDRS "./Wintab/*.h")
FILE(GLOB PCH_HDRS "./PCH/*.h")

INCLUDE_DIRECTORIES (
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/EngineCore
   ${CMAKE_CURRENT_SOURCE_DIR}/GPU2DOperation
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldBrush
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldBuffer
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldFileCodecs
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldGeom
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldOperation
   ${CMAKE_CURRENT_SOURCE_DIR}/HeightfieldSerializer
   ${CMAKE_CURRENT_SOURCE_DIR}/Input
   ${CMAKE_CURRENT_SOURCE_DIR}/PCH
   ${CMAKE_CURRENT_SOURCE_DIR}/Tinyxml
   ${CMAKE_CURRENT_SOURCE_DIR}/Utils
   ${CMAKE_CURRENT_SOURCE_DIR}/Wintab)

set (CPP_FILES
   ${EngineCore_SRCS} 
    ${GPU2DOperation_SRCS} 
    ${HeightfieldBrush_SRCS} 
    ${HeightfieldBuffer_SRCS} 
    ${HeightfieldBuffer_SRCS} 
    ${HeightfieldFileCodecs_SRCS} 
    ${HeightfieldGeom_SRCS} 
    ${HeightfieldOperation_SRCS} 
    ${HeightfieldSerializer_SRCS} 
    ${Input_SRCS}
    ${Input_DEFS}
    ${Tinyxml_SRCS} 
    ${Utils_SRCS}
    #${Wintab_SRCS}
    ${PCH_SRCS}
)
set (HPP_FILES
   ${EngineCore_HDRS} 
    ${GPU2DOperation_HDRS} 
    ${HeightfieldBrush_HDRS} 
    ${HeightfieldBuffer_HDRS} 
    ${HeightfieldBuffer_HDRS} 
    ${HeightfieldFileCodecs_HDRS} 
    ${HeightfieldGeom_HDRS} 
    ${HeightfieldOperation_HDRS} 
    ${HeightfieldSerializer_HDRS} 
    ${Input_HDRS}
    ${Utils_HDRS}
    ${Tinyxml_HDRS}
    #${Wintab_HDRS} 
    ${PCH_HDRS}
)

# NEED TO FINISH PORTING BEFORE ENABLING THIS:
#foreach(src_file ${CPP_FILESa})
#   if (${src_file} MATCHES "stdafx.cpp$")
#       set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")        
#   else()
#       get_filename_component(basename ${src_file} NAME)
#       set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/Ycstdafx.h -DDEBUG_CPP_NAME=\"\\\"${basename}\"\\\"")
#   endif()
#endforeach()
#SetupCompileFlagsWithPCH()

add_library( Engine STATIC ${CPP_FILES} ${HPP_FILES} )

set_target_properties(Engine PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(Engine ${Boost_SYSTEM_LIBRARY} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES})
 
