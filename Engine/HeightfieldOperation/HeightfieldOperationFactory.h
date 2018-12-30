/**
 * ScapeEngine::HeightfieldOperationFactory class
 *
 * Factory for all HeightfieldOperation classes
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONFACTORY_H__
#define __HEIGHTFIELDOPERATIONFACTORY_H__

#include "ScapeEnginePrerequisites.h"

#define REGISTER_HEIGHTFIELDOPERATION_CLASS(factoryObject, className)                                      \
    {                                                                                                      \
        factoryObject->registerClass(className::getStaticClassName(), className::create);                  \
    }

#include "HeightfieldOperation.h"

namespace ScapeEngine
{
class HeightfieldOperationFactory
{

public:
    HeightfieldOperationFactory() {}

    void registerClasses();

    void registerClass(const string& className, HeightfieldOperation* (*classCreateMethod)());

    bool isRegistered(const string& className);

    StringList getOperationNameList();

    HeightfieldOperation* create(const string& className);

protected:
    typedef std::map<string, HeightfieldOperation* (*)()> OperationDescMap;
    OperationDescMap mOperationDescMap;
};
}

#endif // __HEIGHTFIELDOPERATIONMANAGER_H__
