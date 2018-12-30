/**
 * ScapeEngine::HeightfieldOperationStack class
 *
 * Stack of executed operations plus possibly one executing operation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONSTACK_H__
#define __HEIGHTFIELDOPERATIONSTACK_H__

#include "ScapeEnginePrerequisites.h"

#include "EngineCore/Tickable.h"

namespace ScapeEngine
{

class HeightfieldOperationStack : public Tickable
{

public:
    HeightfieldOperationStack();

    ~HeightfieldOperationStack();

    HeightfieldOperation* getNewOperation();

    HeightfieldOperation* getLastOperation();

    virtual void onPreFrameTick();

    bool setNewOperationClassName(const string& className);

    void clearAll();

protected:
    typedef std::deque<HeightfieldOperation*> Operations;
    Operations mOperationHistory;

    HeightfieldOperation* mNewOperation;
    string mNewOperationClassName;

    int mLastOperationIndex;

    HeightfieldOperation* getOperation(size_t index);

    bool doOperation();

    bool undoOperation();

    bool redoOperation();
};
}

#endif // __HEIGHTFIELDOPERATIONSTACK_H__
