/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "TabletInput.h"
#include "TabletInputWin32.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
TabletInput* TabletInput::create(const string& inputWindow)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    return new TabletInputWin32(inputWindow);
#else
    return new TabletInput();
#endif
}
