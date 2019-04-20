#ifndef __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__
#define __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "ButtonDefinitionDataAccessObject.h"

#include <string>

#include "Utils/QtJSONFileHelper.h"

namespace ScapeEngine
{

class QtJSONButtonDefinitionDataAccesObject : public ButtonDefinitionDataAccessObject
{
public:
    QtJSONButtonDefinitionDataAccesObject(std::string fileName);
    virtual ~QtJSONButtonDefinitionDataAccesObject();

    virtual const ButtonDefinitions getButtonDefinitions() const;

protected:
    QtJSONFileHelper mFileHelper;
};
}

#endif /* __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__ */
