/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include <OgrePlatform.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "PCH/stdafx.h"
#include "Wintab/wintab.h"

#define PACKETDATA (PK_CURSOR | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE 0
#define NPACKETQSIZE 1024

#include <Wintab/pktdef.h>

#include "TabletInputWin32.h"

typedef UINT(API* WTINFO)(UINT, UINT, LPVOID);
typedef BOOL(API* WTQUEUESIZESET)(HCTX, int);
typedef HCTX(API* WTOPEN)(HWND, LPLOGCONTEXTA, BOOL);
typedef BOOL(API* WTCLOSE)(HCTX);
typedef BOOL(API* WTGET)(HCTX, LPLOGCONTEXTA);
typedef int(API* WTPACKETSGET)(HCTX, int, LPVOID);

class WinTabLibrary
{
public:
    WinTabLibrary() : mHandle(NULL), mTriedLoad(false) {}
    ~WinTabLibrary() { FreeLibrary(mHandle); }

    HMODULE get()
    {
        if (!mTriedLoad)
        {
            Ogre::LogManager::getSingleton().logMessage(_T("Loading Wintab32.dll..."));
            mHandle = LoadLibrary(_T("Wintab32.dll"));
            if (mHandle)
            {
                Ogre::LogManager::getSingleton().logMessage(_T("Loaded Wintab32.dll."));
            }
            else
            {
                Ogre::LogManager::getSingleton().logMessage(_T("Unable to load Wintab32.dll."));
            }

            mTriedLoad = true;
        }
        return mHandle;
    }

protected:
    HMODULE mHandle;
    bool mTriedLoad;
};

WTINFO pWTInfo = NULL;
WTQUEUESIZESET pWTQueueSizeSet = NULL;
WTOPEN pWTOpen = NULL;
WTCLOSE pWTClose = NULL;
WTGET pWTGet = NULL;
WTPACKETSGET pWTPacketsGet = NULL;
static WinTabLibrary sWinTabLibrary;

// ----------------------------------------------------------------------------
static bool sIsWinTabAvailable = false;

static void tryLoadWinTabLibrary()
{
    if (sWinTabLibrary.get())
    {
        pWTInfo = (WTINFO)GetProcAddress(sWinTabLibrary.get(), _T("WTInfoA"));
        if (!pWTInfo)
            return;
        pWTQueueSizeSet = (WTQUEUESIZESET)GetProcAddress(sWinTabLibrary.get(), _T("WTQueueSizeSet"));
        if (!pWTQueueSizeSet)
            return;
        pWTOpen = (WTOPEN)GetProcAddress(sWinTabLibrary.get(), _T("WTOpenA"));
        if (!pWTOpen)
            return;
        pWTClose = (WTCLOSE)GetProcAddress(sWinTabLibrary.get(), _T("WTClose"));
        if (!pWTClose)
            return;
        pWTGet = (WTGET)GetProcAddress(sWinTabLibrary.get(), _T("WTGetA"));
        if (!pWTGet)
            return;
        pWTPacketsGet = (WTPACKETSGET)GetProcAddress(sWinTabLibrary.get(), _T("WTPacketsGet"));
        if (!pWTPacketsGet)
            return;
        sIsWinTabAvailable = true;
        Ogre::LogManager::getSingleton().logMessage(_T("Found compatible tablet driver."));
    }
}

using namespace ScapeEngine;

static PACKET sPacketBuffer[NPACKETQSIZE];

// ----------------------------------------------------------------------------
TabletInputWin32::TabletInputWin32(const string& inputWindow)
    : mConnected(false), mInputWindow(inputWindow), mCurrentCursorId((Ogre::uint32)-1),
      mContextHandle(NULL), mPressure(-1.0f), mPacketQueueSize(0)
{
}

// ----------------------------------------------------------------------------
TabletInputWin32::~TabletInputWin32() {}

// ----------------------------------------------------------------------------
void TabletInputWin32::connect()
{
    tryLoadWinTabLibrary();

    /* check if WinTab available. */
    if (sIsWinTabAvailable && !mConnected)
    {
        if (pWTInfo(0, 0, NULL))
        {
            LOGCONTEXT logContext;

            /* get default system context. */
            UINT success = pWTInfo(WTI_DEFSYSCTX, 0, &logContext);
            if (success)
            {
                /* same define constants from top of file used in context definition. */
                logContext.lcPktData = PACKETDATA;
                logContext.lcPktMode = PACKETMODE;
                logContext.lcMoveMask = PACKETDATA;

                /* inherit default button down mask. */
                /* allows tablet control panel to reserve some buttons for special */
                /* functions. */
                /* make sure we get button up AND down reports for all buttons we watch. */
                logContext.lcBtnUpMask = logContext.lcBtnDnMask;

                /* open the context */
                HWND hWnd = (HWND)Ogre::StringConverter::parseLong(mInputWindow);
                mContextHandle = pWTOpen(hWnd, &logContext, TRUE);

                if (mContextHandle)
                {
                    /* set queue size */
                    mPacketQueueSize = NPACKETQSIZE;
                    while (mPacketQueueSize > 0)
                    {
                        if (pWTQueueSizeSet(mContextHandle, mPacketQueueSize))
                        {
                            break;
                        }
                        mPacketQueueSize = (mPacketQueueSize >> 1) + (mPacketQueueSize >> 2);
                    }

                    mConnected = true;
                    Ogre::LogManager::getSingleton().logMessage(_T("Connected to tablet driver."));
                }
                else
                {
                    Ogre::LogManager::getSingleton().logMessage(
                        _T("Error during tablet connection setup: Device context unavailable"));
                }
            }
            else
            {
                Ogre::LogManager::getSingleton().logMessage(
                    _T("Error during tablet connection setup: Device interface unavailablr"));
            }
        }
    }
}

// ----------------------------------------------------------------------------
void TabletInputWin32::disconnect()
{
    if (mConnected && mContextHandle)
    {
        pWTClose(mContextHandle);
    }
}

// ----------------------------------------------------------------------------
void TabletInputWin32::cacheCursorSettings()
{
    if (mConnected)
    {
        /* browse WinTab's many info items to discover pressure handling. */
        AXIS pressureAxis;
        LOGCONTEXT logContext;
        BYTE physicalToLogicalButtonMap[32];
        UINT buttonThresholds[2];
        UINT buttonMapSize;

        /* discover the LOGICAL button generated by the pressure channel. */
        /* get the PHYSICAL button from the cursor category and run it */
        /* through that cursor's button map (usually the identity map). */
        mPressureButton = (BYTE)-1;
        pWTInfo(WTI_CURSORS + mCurrentCursorId, CSR_NPBUTTON, &mPressureButton);
        buttonMapSize = pWTInfo(WTI_CURSORS + mCurrentCursorId, CSR_BUTTONMAP, &physicalToLogicalButtonMap);
        if ((UINT)mPressureButton < buttonMapSize)
            mPressureButton = physicalToLogicalButtonMap[mPressureButton];

        /* get the current context for its device variable. */
        pWTGet(mContextHandle, &logContext);

        /* get the buttonMapSize of the pressure axis. */
        pWTInfo(WTI_DEVICES + logContext.lcDevice, DVC_NPRESSURE, &pressureAxis);
        // prsNoBtnOrg = (UINT)pressureAxis.axMin;
        // prsNoBtnExt = (UINT)pressureAxis.axMax - (UINT)pressureAxis.axMin;

        /* get the button marks (up & down generation thresholds) */
        /* and calculate what pressure range we get when pressure-button is down. */
        buttonThresholds[1] = 0; /* default if info item not present. */
        pWTInfo(WTI_CURSORS + mCurrentCursorId, CSR_NPBTNMARKS, buttonThresholds);
        mPressureValueMin = (Ogre::Real)buttonThresholds[1];
        mPressureValueMax = (Ogre::Real)(UINT)pressureAxis.axMax;
    }
}

/* -------------------------------------------------------------------------- */
void TabletInputWin32::capture()
{
    if (mConnected)
    {
        int nPackets = pWTPacketsGet(mContextHandle, mPacketQueueSize, &sPacketBuffer);

        // std::cout << (int)mContextHandle << " " << (int)mPacketQueueSize << " " << (int)sPacketBuffer <<
        // std::endl;

        for (int i = 0; i < nPackets; i++)
        {
            PACKET& packet = sPacketBuffer[i];

            if (mCurrentCursorId != packet.pkCursor)
            {
                mCurrentCursorId = packet.pkCursor;
                /* re-init on cursor change. */
                cacheCursorSettings();
            }

            if (packet.pkButtons & (1 << mPressureButton))
            {
                mPressure = ((Ogre::Real)packet.pkNormalPressure - mPressureValueMin) /
                            (mPressureValueMax - mPressureValueMin);
                mPressure = Utils::clamp(mPressure, 0.0f, 1.0f);
            }
            else
            {
                mPressure = -1.0f;
            }
            // std::cout << mPressure << std::endl;
        }
    }
}

#endif
