/**
 * Precompiled header includes
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __STDAFXENGINE_H__
#define __STDAFXENGINE_H__

#pragma warning(push, 3)

#define _CRT_SECURE_NO_DEPRECATE 
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#define NOMINMAX

#include "Ogre.h"
//#include "OgreNoMemoryMacros.h"
//#include "OgreMemoryMacros.h"
#include "OgreConfigFile.h"
#include "OgreException.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"
#include "OIS/OIS.h"
#include "Tinyxml/tinyxml.h"
#include <string>
#include <list>
#include <map>
#include <vector>

typedef std::string string;
#ifndef _T
#define _T(x) x
#endif
#ifndef _TCHAR
#define _TCHAR char
#endif

#include "Utils/Utils.h"
#include "EngineCore/EngineCore.h"

#pragma warning(pop)

#pragma warning(disable:4100) //unreferenced formal parameter

#endif // __STDAFXENGINE_H__
