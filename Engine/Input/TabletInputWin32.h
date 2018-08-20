/**
 * ScapeEngine::TabletInputWin32 class
 *
 * Tablet interface implementation for Windows
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TABLETINPUTWIN32_H__
#define __TABLETINPUTWIN32_H__

#include <OgrePlatform.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WINDOWS

#include "TabletInput.h"
#include "Wintab/wintab.h"

namespace ScapeEngine {
    class TabletInputWin32 : public TabletInput
    {
    public:
        TabletInputWin32(const string& inputWindow);

        virtual ~TabletInputWin32();

        virtual void connect();

        virtual bool isConnected() { return mConnected; }
        virtual float getPressure() { return mPressure; }
        virtual void disconnect();

        virtual void capture();

    private:
        bool mConnected;
        int mPacketQueueSize;
        string mInputWindow;
        HCTX mContextHandle;
        Ogre::uint32 mCurrentCursorId;
        Ogre::uint8 mPressureButton;
        Ogre::Real mPressureValueMin, mPressureValueMax;
        Ogre::Real mPressure;

        void cacheCursorSettings();
    };
}

#endif

#endif // __TABLETINPUTWIN32_H__
