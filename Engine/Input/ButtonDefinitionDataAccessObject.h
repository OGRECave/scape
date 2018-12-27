#ifndef __BUTTONDEFINITIONDATAACCESSOBJECT_H__
#define __BUTTONDEFINITIONDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include <vector>

namespace ScapeEngine
{

class ButtonDefinition;

class ButtonDefinitionDataAccessObject
{
public:
    typedef std::vector<ButtonDefinition> ButtonDefinitions;

    virtual ~ButtonDefinitionDataAccessObject();

    virtual const ButtonDefinitions getButtonDefinitions() const = 0;

protected:
    ButtonDefinitionDataAccessObject();
};
}

#endif /* __BUTTONDEFINITIONDATAACCESSOBJECT_H__ */
