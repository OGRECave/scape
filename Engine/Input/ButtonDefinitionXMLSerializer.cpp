/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"

#include "InputManager.h"
#include "ButtonDefinitionXMLSerializer.h"

namespace ScapeEngine
{

ButtonDefinitionXMLSerializer::ButtonDefinitionXMLSerializer(std::string fileName,
                                                             std::string resourceGroupName)
    : mFileName(fileName), mResourceGroupName(resourceGroupName)
{
}

ButtonDefinitionXMLSerializer::~ButtonDefinitionXMLSerializer() {}

const ButtonDefinitionDataAccessObject::ButtonDefinitions
ButtonDefinitionXMLSerializer::getButtonDefinitions() const
{
    Ogre::LogManager::getSingleton().logMessage(
        _T("ButtonDefinitionXMLReader::importXML: reading XML data from ") + mFileName + _T("..."));

    string contents;
    Utils::readResource(mFileName, mResourceGroupName, contents);
    const ButtonDefinitionDataAccessObject::ButtonDefinitions ret = importXML(contents);

    return ret;
}

const ButtonDefinitionDataAccessObject::ButtonDefinitions
ButtonDefinitionXMLSerializer::importXML(const string& rawData) const
{
    ButtonDefinitionDataAccessObject::ButtonDefinitions ret;

    Ogre::LogManager::getSingleton().logMessage(_T("ButtonDefinitionXMLReader::importXML: parsing..."));

    TiXmlDocument tiXMLDoc(_T(""));
    tiXMLDoc.Parse(rawData.c_str());

    if (tiXMLDoc.Error())
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("ButtonDefinitionXMLReader::importXML: TinyXML error ") +
            Ogre::StringConverter::toString(tiXMLDoc.ErrorId()) + _T(": ") + tiXMLDoc.ErrorDesc());
        return ret;
    }

    const TiXmlElement *elem, *rootElem = tiXMLDoc.RootElement();

    elem = rootElem->FirstChildElement(_T("BUTTONS"));
    if (elem)
    {
        for (const TiXmlElement* buttonElem = elem->FirstChildElement(_T("BUTTON")); buttonElem;
             buttonElem = buttonElem->NextSiblingElement(_T("BUTTON")))
        {
            _readElementBUTTON(*buttonElem, ret);
        }
    }

    Ogre::LogManager::getSingleton().logMessage(_T("InputManager::importButtonDefinitions: Finished."));

    return ret;
}

void ButtonDefinitionXMLSerializer::_readElementBUTTON(const TiXmlElement& buttonElem,
                                                       ButtonDefinitions& definitions)
{
    const char* buttonIdString = buttonElem.Attribute(_T("id"));
    if (!buttonIdString)
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("ButtonDefinitionXMLReader::importXML: missing 'id' attribute of 'BUTTON' element"));
        return;
    }
    ButtonId::EButtonId buttonId = ButtonId::getButtonIdFromUpperName(buttonIdString);

    const char* priorityString = buttonElem.Attribute(_T("priority"));
    if (!priorityString)
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("ButtonDefinitionXMLReader::importXML: missing 'priority' attribute of 'BUTTON' element"));
        return;
    }
    int priority = Ogre::StringConverter::parseInt(priorityString);

    for (const TiXmlElement* buttonDefinitionElem = buttonElem.FirstChildElement(_T("DEF"));
         buttonDefinitionElem; buttonDefinitionElem = buttonDefinitionElem->NextSiblingElement(_T("DEF")))
    {
        _readElementDEF(*buttonDefinitionElem, definitions, buttonId, priority);
    }
}

void ButtonDefinitionXMLSerializer::_readElementDEF(const TiXmlElement& buttonDefinitionElem,
                                                    ButtonDefinitions& definitions,
                                                    ButtonId::EButtonId buttonId, int priority)
{
    ButtonDefinition buttonDefinition(buttonId, priority);

    for (const TiXmlElement* deviceButtonElem = buttonDefinitionElem.FirstChildElement(_T("DBUTTON"));
         deviceButtonElem; deviceButtonElem = deviceButtonElem->NextSiblingElement(_T("DBUTTON")))
    {
        _readElementDBUTTON(*deviceButtonElem, buttonDefinition);
    }

    definitions.push_back(buttonDefinition);
}

void ButtonDefinitionXMLSerializer::_readElementDBUTTON(const TiXmlElement& deviceButtonElem,
                                                        ButtonDefinition& buttonDefinition)
{
    const char* deviceButtonIdString = deviceButtonElem.Attribute(_T("id"));
    if (!deviceButtonIdString)
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("ButtonDefinitionXMLReader::importXML: missing 'id' attribute of 'DEVICEBUTTON' element"));
        return;
    }

    DeviceButtonId::EDeviceButtonId deviceButtonId =
        DeviceButtonId::getDeviceButtonIdFromUpperName(deviceButtonIdString);
    buttonDefinition.addDeviceButton(deviceButtonId);
}
}
