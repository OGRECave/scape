#include "PCH/stdafx.h"
#include "InputManager.h"

#include "QtJSONButtonDefinitionDataAccesObject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace ScapeEngine
{

QtJSONButtonDefinitionDataAccesObject::QtJSONButtonDefinitionDataAccesObject(std::string fileName,
                                                                             std::string resourceGroupName)
    : mFileName(fileName), mResourceGroupName(resourceGroupName)
{
}

QtJSONButtonDefinitionDataAccesObject::~QtJSONButtonDefinitionDataAccesObject() {}

const ButtonDefinitionDataAccessObject::ButtonDefinitions
QtJSONButtonDefinitionDataAccesObject::getButtonDefinitions() const
{
    Ogre::LogManager::getSingleton().logMessage(
        "QtJSONButtonDefinitionDataAccesObject: Reading JSON from " + mFileName + "...");

    ButtonDefinitionDataAccessObject::ButtonDefinitions ret;

    std::vector<char> contents;
    Ogre::DataStreamPtr pStream =
        Ogre::ResourceGroupManager::getSingleton().openResource(mFileName, mResourceGroupName);
    size_t size = pStream->size();
    if (size > 0)
    {
        contents.resize(size);
        if (pStream->read(contents.data(), size) != size)
        {
            Ogre::LogManager::getSingleton().logMessage(
                "QtJSONButtonDefinitionDataAccesObject: Failed to read JSON from " + mFileName);
            return ret;
        }
    }

    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(QByteArray(contents.data(), contents.size()), &errorStruct);
    if (errorStruct.error != QJsonParseError::NoError)
    {
        Ogre::LogManager::getSingleton().logMessage(
            "QtJSONButtonDefinitionDataAccesObject: Error while reading JSON from " + mFileName + " : " +
            errorStruct.errorString().toStdString());
    }
    else
    {
        if (jsonDoc.isObject())
        {
            QJsonObject rootObject = jsonDoc.object();

            if (rootObject.contains("ButtonDefinitions"))
            {
                QJsonValue definitionsValue = rootObject["ButtonDefinitions"];
                if (definitionsValue.isObject())
                {
                    QJsonObject definitions = definitionsValue.toObject();
                    for (QJsonObject::iterator defIt = definitions.begin(); defIt != definitions.end();
                         defIt++)
                    {
                        QString defKey = defIt.key();
                        QJsonValue defValue = defIt.value();
                        if (defValue.isObject())
                        {
                            QJsonObject definition = defValue.toObject();
                            if (definition.contains("priority") && definition.contains("definitions"))
                            {
                                QJsonValue priorityValue = definition["priority"];
                                QJsonValue buttonDefinitionsValue = definition["definitions"];

                                if (priorityValue.isString() && buttonDefinitionsValue.isArray())
                                {
                                    QString priorityStr = priorityValue.toString();
                                    int priority = 0;
                                    priority = priorityStr.toInt();

                                    ButtonId::EButtonId buttonId =
                                        ButtonId::getButtonIdFromUpperName(defKey.toStdString());

                                    QJsonArray buttonDefinitionsArray = buttonDefinitionsValue.toArray();
                                    for (QJsonArray::iterator butDefIt = buttonDefinitionsArray.begin();
                                         butDefIt != buttonDefinitionsArray.end(); butDefIt++)
                                    {
                                        ButtonDefinition buttonDefinition(buttonId, priority);

                                        QJsonValue curVal = *butDefIt;
                                        if (curVal.isArray())
                                        {
                                            QJsonArray buttonDefinitionsSubArray = curVal.toArray();
                                            for (QJsonArray::iterator butDefSubIt =
                                                     buttonDefinitionsSubArray.begin();
                                                 butDefSubIt != buttonDefinitionsSubArray.end();
                                                 butDefSubIt++)
                                            {
                                                QJsonValue curSubVal = *butDefSubIt;
                                                if (curSubVal.isString())
                                                {
                                                    QString curSubValStr = curSubVal.toString();

                                                    DeviceButtonId::EDeviceButtonId deviceButtonId =
                                                        DeviceButtonId::getDeviceButtonIdFromUpperName(
                                                            curSubValStr.toStdString());
                                                    buttonDefinition.addDeviceButton(deviceButtonId);
                                                }
                                            }
                                        }

                                        ret.push_back(buttonDefinition);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}
}
