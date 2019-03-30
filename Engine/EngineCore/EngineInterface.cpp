/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"

#include "Console.h"
#include "EngineInterface.h"
#include "SettingsDatasetManager.h"
#include "RenderViewManager.h"
#include "RenderView.h"
#include "SkySettings.h"
#include "Input/InputManager.h"
#include "HeightfieldOperation/HeightfieldOperation.h"
#include "HeightfieldOperation/HeightfieldOperationStack.h"
#include "HeightfieldOperation/HeightfieldOperationFactory.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"
#include "HeightfieldGeom/HeightfieldGeomMaterial.h"
// TODO GdCarpentier 1/23/2008 : remove
//#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldFileCodecs/HeightfieldFileEncoderManager.h"
#include "HeightfieldFileCodecs/HeightfieldFileDecoderManager.h"
#include "HeightfieldFileCodecs/HeightfieldFileEncoder.h"
#include "HeightfieldFileCodecs/HeightfieldFileDecoder.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include <iostream>

using namespace ScapeEngine;

void EngineInterface::initialize()
{
    new EngineCore(this);
    getEngineCore()->initialize();
}

void EngineInterface::deinitialize()
{
    getEngineCore()->deinitialize();
    delete EngineCore::getSingletonPtr();
}

void EngineInterface::attachInputToWindow(std::string inputWindow)
{
    getEngineCore()->attachInputToWindow(inputWindow);
}

void EngineInterface::createRenderView(int viewId, const std::string& windowHandle, int left, int top,
                                       int width, int height)
{
    getEngineCore()->getRenderViewManager()->createRenderView(viewId, windowHandle, left, top, width,
                                                              height);
}

void EngineInterface::update() { getEngineCore()->update(); }

void EngineInterface::onRenderViewMovedOrResized(int viewId, int left, int top, int width, int height)
{
    getEngineCore()->getRenderViewManager()->onRenderViewMovedOrResized(viewId, left, top, width, height);
}

void EngineInterface::onRenderViewSetFocus(int viewId)
{
    getEngineCore()->getInputManager()->onRenderViewSetFocus(viewId);
}

void EngineInterface::onRenderViewKillFocus(int viewId)
{
    getEngineCore()->getInputManager()->onRenderViewKillFocus(viewId);
}

void EngineInterface::detachRenderView(int viewId)
{
    getEngineCore()->getRenderViewManager()->detachRenderView(viewId);
}

void EngineInterface::openConsole() { Console::openConsole(); }

bool EngineInterface::onFatalException()
{
    try
    {
        throw;
    }
    catch (Ogre::Exception& e)
    {

        Ogre::StringStream stream;
        stream << _T("A fatal error occured:\n") << e.getFullDescription();
        std::cout << stream.str();
        Ogre::LogManager::getSingleton().logMessage(stream.str());
        // MessageBox(NULL, stream.str().c_str(), _T("A fatal exception has occured!"), MB_OK | MB_ICONERROR
        // | MB_TASKMODAL);
        assert(false && _T("Exception!"));
        return true;
    }
    catch (std::exception& e)
    {
        Ogre::StringStream stream;
        stream << _T("A fatal error occured:\n") << e.what();
        std::cout << stream.str();
        Ogre::LogManager::getSingleton().logMessage(stream.str());
        // MessageBox(NULL, stream.str().c_str(), _T("A fatal exception has occured!"), MB_OK | MB_ICONERROR
        // | MB_TASKMODAL);
        assert(false && _T("Exception!"));
        return true;
    }
    catch (...)
    {
        assert(false && _T("Exception!"));
    }
    return false;
}

bool EngineInterface::selectOperation(const string& operationName)
{
    if (getEngineCore()->getHeightfieldOperationStack()->setNewOperationClassName(operationName))
    {
        return true;
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("EngineInterface::selectOperation(): unknown specified operation '") + operationName +
            _T("'"));
        return false;
    }
}

string EngineInterface::getUIElementPropertyField(const string& elementName, const string& propertyName,
                                                  const string& propertyFieldName)
{
    return getEngineCore()->getSettingsDatasetManager()->getSetting(_T("UI"), elementName, propertyName,
                                                                    propertyFieldName);
}

StringStringStringPairMap EngineInterface::getFileFilterMap(EScapeUIElementGroupId groupId)
{
    switch (groupId)
    {
    case SCAPEUIELEMENTGROUPID_FILEEXPORT:
    {
        return getEngineCore()->getHeightfieldFileEncoderManager()->getFileFilterMap();
    }
    case SCAPEUIELEMENTGROUPID_FILEIMPORT:
    {
        return getEngineCore()->getHeightfieldFileDecoderManager()->getFileFilterMap();
    }
    }

    return StringStringStringPairMap();
}

StringStringMap EngineInterface::getUIElementPropertyValueMap(EScapeUIElementGroupId groupId,
                                                              const string& elementName)
{
    UIElementContainer* container = getUIElementContainer(groupId, elementName);

    if (container)
    {
        return container->getUIElementPropertyFieldValueMap();
    }

    return StringStringMap();
}

string EngineInterface::getUIElementPresetPropertyValue(EScapeUIElementGroupId groupId,
                                                        const string& elementName, const string& presetName,
                                                        const string& propertyName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));

    if (container)
    {
        return container->getUIElementPresetPropertyValue(presetName, propertyName);
    }

    return Utils::emptyString;
}

void EngineInterface::setUIElementPresetPropertyValue(EScapeUIElementGroupId groupId,
                                                      const string& elementName, const string& presetName,
                                                      const string& propertyName, const string& value)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));

    if (container)
    {
        container->setUIElementPresetPropertyValue(presetName, propertyName, value);
    }
}

StringStringMap EngineInterface::getUIElementPresetPropertyValueMap(EScapeUIElementGroupId groupId,
                                                                    const string& elementName,
                                                                    const string& presetName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        return container->getUIElementPresetPropertyValueMap(presetName);
    }
    return StringStringMap();
}

void EngineInterface::setUIElementPresetPropertyValueMap(EScapeUIElementGroupId groupId,
                                                         const string& elementName,
                                                         const string& presetName,
                                                         const StringStringMap& valueMap)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        container->setUIElementPresetPropertyValueMap(presetName, valueMap);
    }
}

StringStringMap EngineInterface::setUIElementPropertyValueMap(EScapeUIElementGroupId groupId,
                                                              const string& elementName,
                                                              const StringStringMap& valueMap)
{
    UIElementContainer* container = getUIElementContainer(groupId, elementName);

    if (container)
    {
        return container->setUIElementPropertyValueMap(valueMap);
    }

    return StringStringMap();
}

StringList EngineInterface::getUIElementPresetPropertyNames(EScapeUIElementGroupId groupId,
                                                            const string& elementName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));

    if (container)
    {
        return container->getUIElementPresetPropertyNames();
    }

    return StringList();
}

void EngineInterface::deleteUIElementPreset(EScapeUIElementGroupId groupId, const string& elementName,
                                            const string& presetName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        container->deleteUIElementPreset(presetName);
    }
}

string EngineInterface::importUIElementPreset(EScapeUIElementGroupId groupId, const string& elementName,
                                              const string& fileName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        return container->importUIElementPreset(fileName);
    }
    return Utils::emptyString;
}

void EngineInterface::exportUIElementPreset(EScapeUIElementGroupId groupId, const string& elementName,
                                            const string& fileName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        container->exportUIElementPreset(fileName);
    }
}

string EngineInterface::makeUniquePresetName(EScapeUIElementGroupId groupId, const string& elementName,
                                             const string& baseName)
{
    UIElementPresetContainer* container =
        dynamic_cast<UIElementPresetContainer*>(getUIElementContainer(groupId, elementName));
    if (container)
    {
        return container->makeUniquePresetName(baseName);
    }
    return Utils::emptyString;
}

bool EngineInterface::exportImageFile(const string& encoderName, const string& fileName, string* error)
{
    HeightfieldGeom* geom = getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
    if (geom)
    {
        HeightfieldBuffer* buffer = geom->getHeightfieldBuffer();

        HeightfieldFileEncoder* encoder =
            getEngineCore()->getHeightfieldFileEncoderManager()->getEncoder(encoderName);
        if (encoder)
        {
            return encoder->encode(buffer, fileName, error);
        }
    }
    return false;
}

bool EngineInterface::importImageFile(const string& decoderName, const string& fileName, string* error)
{
    HeightfieldGeom* geom = getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
    if (geom)
    {
        HeightfieldBuffer* buffer = geom->getHeightfieldBuffer();

        HeightfieldFileDecoder* decoder =
            getEngineCore()->getHeightfieldFileDecoderManager()->getDecoder(decoderName);
        if (decoder)
        {
            if (decoder->decode(buffer, fileName, error))
            {
                getEngineCore()->getHeightfieldOperationStack()->clearAll();
                return true;
            }
        }
    }
    return false;
}

string EngineInterface::getDecoderNameFromExtension(const string& extension)
{
    return getEngineCore()->getHeightfieldFileDecoderManager()->getDecoderNameFromExtension(extension);
}

StringList EngineInterface::getUIElementNameList(EScapeUIElementGroupId groupId)
{
    StringList list;

    switch (groupId)
    {
    case SCAPEUIELEMENTGROUPID_OPERATION:
    {
        list = getEngineCore()->getHeightfieldOperationFactory()->getOperationNameList();
    }
    break;

    case SCAPEUIELEMENTGROUPID_MATERIAL:
    {
        list.push_back(_T("MaterialSettings"));
    }
    break;

    case SCAPEUIELEMENTGROUPID_SKY:
    {
        list.push_back(_T("SkySettings"));
    }
    break;

    case SCAPEUIELEMENTGROUPID_RENDERWINDOW:
    {
        list.push_back(_T("RenderWindowSettings"));
    }
    break;

    case SCAPEUIELEMENTGROUPID_FILEEXPORT:
    {
        list = getEngineCore()->getHeightfieldFileEncoderManager()->getEncoderNameList();
    }
    break;

    case SCAPEUIELEMENTGROUPID_FILEIMPORT:
    {
        list = getEngineCore()->getHeightfieldFileDecoderManager()->getDecoderNameList();
    }
    break;
    }

    return list;
}

StringList EngineInterface::getUIElementPropertyNameList(EScapeUIElementGroupId groupId,
                                                         const string& elementName)
{
    StringList list;

    UIElementContainer* container = getUIElementContainer(groupId, elementName);
    if (container)
    {
        list = container->getUIElementPropertyFieldNameList();
    }

    return list;
}

string EngineInterface::getUIElementPropertyValue(EScapeUIElementGroupId groupId, const string& elementName,
                                                  const string& propertyName)
{
    UIElementContainer* container = getUIElementContainer(groupId, elementName);

    if (container)
    {
        return container->getUIElementPropertyFieldValue(propertyName);
    }

    return Utils::emptyString;
}

string EngineInterface::setUIElementPropertyValue(EScapeUIElementGroupId groupId, const string& elementName,
                                                  const string& propertyName, const string& value)
{
    UIElementContainer* container = getUIElementContainer(groupId, elementName);

    if (container)
    {
        return container->setUIElementPropertyValue(propertyName, value);
    }

    return value;
}

UIElementContainer* EngineInterface::getUIElementContainer(EScapeUIElementGroupId groupId,
                                                           const string& elementName)
{
    UIElementContainer* container = NULL;

    switch (groupId)
    {
    case SCAPEUIELEMENTGROUPID_OPERATION:
    {
        container = getEngineCore()->getHeightfieldOperationStack()->getNewOperation();
    }
    break;

    case SCAPEUIELEMENTGROUPID_MATERIAL:
    {
        if (elementName.compare(_T("MaterialSettings")) == 0)
        {
            HeightfieldGeom* geom =
                getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
            if (geom)
            {
                container = geom->getHeightfieldGeomMaterial();
            }
        }
    }
    break;

    case SCAPEUIELEMENTGROUPID_SKY:
    {
        if (elementName.compare(_T("SkySettings")) == 0)
        {
            container = getEngineCore()->getSkySettings();
        }
    }
    break;

    case SCAPEUIELEMENTGROUPID_RENDERWINDOW:
    {
        if (elementName.compare(_T("RenderWindowSettings")) == 0)
        {
            container = getEngineCore()->getRenderViewManager()->getRenderView(
                getEngineCore()->getRenderViewManager()->getCurrentRenderViewId());
        }
    }
    break;

    case SCAPEUIELEMENTGROUPID_FILEEXPORT:
    {
        container = getEngineCore()->getHeightfieldFileEncoderManager()->getEncoder(elementName);
    }
    break;

    case SCAPEUIELEMENTGROUPID_FILEIMPORT:
    {
        container = getEngineCore()->getHeightfieldFileDecoderManager()->getDecoder(elementName);
    }
    break;

    default:
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("EngineInterface::getUIElementContainer(): unknown element '") + elementName +
            _T(" in group '") + Ogre::StringConverter::toString(groupId) + _T("'"));
    }
    break;
    }

    return container;
}

HeightfieldBrushSettings& EngineInterface::getHeightfieldBrushSettings()
{
    return getEngineCore()->getHeightfieldBrushManager()->getHeightfieldBrushSettings();
}

StartupSettingsDataAccessObject& EngineInterface::getStartupSettingsDataAccessObject() const
{
    return getEngineCore()->getStartupSettingsDataAccessObject();
}
