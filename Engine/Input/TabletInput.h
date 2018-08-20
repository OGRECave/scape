/**
 * ScapeEngine::TabletInput class
 *
 * Tablet communication interface, specialized by TabletInputWin32
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TABLETINPUT_H__
#define __TABLETINPUT_H__

namespace ScapeEngine {
    class TabletInput
    {
    public:
        static TabletInput* create(const string& inputWindow);

        virtual ~TabletInput() {}
        virtual void connect() {}
        virtual bool isConnected() { return false; }
        virtual void capture() {}
        // if pressure readerings are supported, a non-negative value is returned.
        // if not supported, a negative value is returned
        virtual float getPressure() { return -1.0f; }
        virtual void disconnect() {}
    };
}

#endif // __TABLETINPUT_H__