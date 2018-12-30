#ifndef __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__
#define __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "ButtonDefinitionDataAccessObject.h"

#include <string>

namespace ScapeEngine
{

class QtJSONButtonDefinitionDataAccesObject : public ButtonDefinitionDataAccessObject
{
private:
    std::string mFileName;
    std::string mResourceGroupName;

public:
    QtJSONButtonDefinitionDataAccesObject(std::string fileName, std::string resourceGroupName);
    virtual ~QtJSONButtonDefinitionDataAccesObject();

    virtual const ButtonDefinitions getButtonDefinitions() const;
};
}

#endif /* __QTJSONBUTTONDEFINITIONDATAACCESOBJECT_H__ */
