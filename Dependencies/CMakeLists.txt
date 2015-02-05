cmake_minimum_required(VERSION 2.8.4)

project(Dependencies)

SET(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/lib")

# External Projects
include(ExternalProject)

# Qt PropertyBrowser
ExternalProject_Add(QtSolutions_PropertyBrowser
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  HG_REPOSITORY https://bitbucket.org/ogitor/ogitor-qtpropertybrowser
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/bin -DCMAKE_DEBUG_POSTFIX=${CMAKE_DEBUG_POSTFIX}
  BUILD_IN_SOURCE 1
)
set_target_properties(QtSolutions_PropertyBrowser PROPERTIES FOLDER Dependencies) 

# Editor Media
ExternalProject_Add(EditorMedia
  URL https://bitbucket.org/ogreaddons/scape/downloads/ScapeMedia.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_BINARY_DIR}/src/EditorMedia" "${CMAKE_BINARY_DIR}/bin"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)
set_target_properties(EditorMedia PROPERTIES FOLDER Assets) 
