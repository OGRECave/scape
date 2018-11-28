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

file(GLOB SRCS "./*/*.cpp")
file(GLOB DEFS "./*/*.def")
file(GLOB HDRS "./*/*.h")

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

add_library(Engine STATIC ${SRCS} ${DEFS} ${HDRS})

set_target_properties(Engine PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(Engine ${OGRE_LIBRARIES})
 
