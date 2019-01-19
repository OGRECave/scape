#ifndef __SCAPEENGINEPLATFORM_H__
#define __SCAPEENGINEPLATFORM_H__

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #define _CRT_SECURE_NO_DEPRECATE
    #define WIN32_LEAN_AND_MEAN
    #define _WIN32_WINNT 0x0501
    #define NOMINMAX

    #include <windows.h>
    #include <direct.h>
#endif

#define _ScapeEngineExport

#endif // __SCAPEENGINEPLATFORM_H__

