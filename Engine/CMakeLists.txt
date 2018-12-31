Project(Engine)

file(GLOB SRCS "./*/*.cpp")
file(GLOB DEFS "./*/*.def")
file(GLOB HDRS "./*/*.h")

add_library(Engine STATIC ${SRCS} ${DEFS} ${HDRS})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(Engine PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(Engine ${OGRE_LIBRARIES} Qt5::Core)
 
