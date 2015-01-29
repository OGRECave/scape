#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
#This source file is part of the
#   ___                 __    __ _ _    _ 
#  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
# //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
#/ \_// (_| | | |  __/  \  /\  /| |   <| |
#\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#      |___/                              
#      Tutorial Framework
#      http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/
cmake_minimum_required(VERSION 2.8)
 
project(Scape)
 
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_INSTALL_PREFIX})

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
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
 
# Find Boost
# Prefer static linking in all cases
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
   if (WIN32 OR APPLE)
      set(Boost_USE_STATIC_LIBS TRUE)
   else ()
      # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
      set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
   endif ()

   if (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)
      set(Boost_USE_MULTITHREADED OFF)
   endif()

   set(Boost_ADDITIONAL_VERSIONS "1.57" "1.57.0" "1.56" "1.56.0" "1.55" "1.55.0" "1.54" "1.54.0" "1.53" "1.53.0" "1.52" "1.52.0" "1.51" "1.51.0" "1.50" "1.50.0" "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.45" "1.45.0" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40")

   # Components that need linking (NB does not include header-only components like bind)
   set(OGRE_BOOST_COMPONENTS thread date_time system regex filesystem)

   find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)

   if (NOT Boost_FOUND)
      # Try again with the other type of libs
      if(Boost_USE_STATIC_LIBS)
         set(Boost_USE_STATIC_LIBS OFF)
      else()
         set(Boost_USE_STATIC_LIBS ON)
      endif()

      find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
   endif()

   if(Boost_FOUND AND Boost_VERSION GREATER 104900)
      if(Boost_VERSION GREATER 105300)
         set(OGRE_BOOST_COMPONENTS thread date_time system atomic chrono regex filesystem)
      else()
         set(OGRE_BOOST_COMPONENTS thread date_time system chrono regex filesystem)
      endif()
      find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
   endif()

   if(Boost_FOUND AND NOT WIN32)
     list(REMOVE_DUPLICATES Boost_LIBRARIES)
   endif()

   if (Boost_FOUND)
      set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
      set(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
   endif (Boost_FOUND)
endif(NOT OGRE_BUILD_PLATFORM_IPHONE)

find_package(OGRE REQUIRED)
 
#if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
#  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
#endif()
 
find_package(OIS REQUIRED)
 
if(NOT OIS_FOUND)
   message(SEND_ERROR "Failed to find OIS.")
endif()

include_directories( ${OIS_INCLUDE_DIRS}
   ${OGRE_INCLUDE_DIRS}
   ${OGRE_SAMPLES_INCLUDEPATH}
   ${OGRE_Overlay_INCLUDE_DIRS}
    Engine
)
 
ADD_SUBDIRECTORY(Engine)
#ADD_SUBDIRECTORY(Editor)
ADD_SUBDIRECTORY(NewEditor)
