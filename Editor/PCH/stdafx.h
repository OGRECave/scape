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
#include "windows.h"

#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/aui/aui.h"

#pragma warning(pop)

#pragma warning(disable:4100) //unreferenced formal parameter


#define SAFE_DELETE(a) {delete a; a = NULL;}

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

#endif // __STDAFXENGINE_H__