/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"

#include "ButtonDefinitionXMLSerializer.h"
#include "InputManager.h"

using namespace ScapeEngine;

static void _readElementBUTTON(const TiXmlElement& buttonElem);
static void _readElementDEF(const TiXmlElement& buttonDefinitionElem, ButtonId::EButtonId buttonId);
static void _readElementDBUTTON(const TiXmlElement& deviceButtonElem, ButtonDefinition& buttonDefinition);

// ----------------------------------------------------------------------------
void ScapeEngine::ButtonDefinitionXMLSerializer::importSceneFromResource(const string& fileName,
                                                                         const string& groupName)
{
    Ogre::LogManager::getSingleton().logMessage(
        _T("ButtonDefinitionXMLReader::importXML: reading XML data from ") + fileName + _T("..."));

    string contents;
    Utils::readResource(fileName, groupName, contents);
    importXML(contents);
}

// ----------------------------------------------------------------------------
void ScapeEngine::ButtonDefinitionXMLSerializer::importXML(const string& rawData)
{
    Ogre::LogManager::getSingleton().logMessage(_T("ButtonDefinitionXMLReader::importXML: parsing..."));

    TiXmlDocument tiXMLDoc(_T(""));
    tiXMLDoc.Parse(rawData.c_str());

    if (tiXMLDoc.Error())
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("ButtonDefinitionXMLReader::importXML: TinyXML error ") +
            Ogre::StringConverter::toString(tiXMLDoc.ErrorId()) + _T(": ") + tiXMLDoc.ErrorDesc());
        return;
    }

    const TiXmlElement *elem, *rootElem = tiXMLDoc.RootElement();

    elem = rootElem->FirstChildElement(_T("BUTTONS"));
    if (elem)
    {
        for (const TiXmlElement* buttonElem = elem->FirstChildElement(_T("BUTTON")); buttonElem;
             buttonElem = buttonElem->NextSiblingElement(_T("BUTTON")))
        {
            _readElementBUTTON(*buttonElem);
        }
    }

    Ogre::LogManager::getSingleton().logMessage(_T("InputManager::importButtonDefinitions: Finished."));
}

// ----------------------------------------------------------------------------
void _readElementBUTTON(const TiXmlElement& buttonElem)
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
        _readElementDEF(*buttonDefinitionElem, buttonId);
    }

    getEngineCore()->getInputManager()->getButton(buttonId)->setPriority(priority);
}

// ----------------------------------------------------------------------------
void _readElementDEF(const TiXmlElement& buttonDefinitionElem, ButtonId::EButtonId buttonId)
{
    ButtonDefinition buttonDefinition(buttonId);

    for (const TiXmlElement* deviceButtonElem = buttonDefinitionElem.FirstChildElement(_T("DBUTTON"));
         deviceButtonElem; deviceButtonElem = deviceButtonElem->NextSiblingElement(_T("DBUTTON")))
    {
        _readElementDBUTTON(*deviceButtonElem, buttonDefinition);
    }

    getEngineCore()->getInputManager()->addButtonDefinition(buttonDefinition);
}

// ----------------------------------------------------------------------------
void _readElementDBUTTON(const TiXmlElement& deviceButtonElem, ButtonDefinition& buttonDefinition)
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
