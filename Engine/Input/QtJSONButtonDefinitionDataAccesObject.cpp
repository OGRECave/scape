#include "ScapeEngineStableHeaders.h"
#include "InputManager.h"

#include "QtJSONButtonDefinitionDataAccesObject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace ScapeEngine
{

QtJSONButtonDefinitionDataAccesObject::QtJSONButtonDefinitionDataAccesObject(std::string fileName)
    : mFileHelper(fileName)
{
}

QtJSONButtonDefinitionDataAccesObject::~QtJSONButtonDefinitionDataAccesObject() {}

const ButtonDefinitionDataAccessObject::ButtonDefinitions
QtJSONButtonDefinitionDataAccesObject::getButtonDefinitions() const
{
    ButtonDefinitionDataAccessObject::ButtonDefinitions ret;

    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc = mFileHelper.readJSONFile(errorStruct);
    if (errorStruct.error == QJsonParseError::NoError)
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

                                    ButtonDefinition buttonDefinition(buttonId, priority);

                                    QJsonArray buttonDefinitionsArray = buttonDefinitionsValue.toArray();
                                    for (QJsonArray::iterator butDefIt = buttonDefinitionsArray.begin();
                                         butDefIt != buttonDefinitionsArray.end(); butDefIt++)
                                    {
                                        QJsonValue curVal = *butDefIt;
                                        if (curVal.isArray())
                                        {
                                            ButtonDefinition::DeviceButtonSet currentDeviceButtons;

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
                                                    currentDeviceButtons.insert(deviceButtonId);
                                                }
                                            }

                                            if (!currentDeviceButtons.empty())
                                            {
                                                buttonDefinition.getDeviceButtonSets().push_back(
                                                    currentDeviceButtons);
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

    return ret;
}

void QtJSONButtonDefinitionDataAccesObject::updateButtonDefinitions(
    const ButtonDefinitionDataAccessObject::ButtonDefinitions& buttonDefinitions)
{
    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc = mFileHelper.readJSONFile(errorStruct);

    if (errorStruct.error == QJsonParseError::NoError)
    {
        if (jsonDoc.isObject())
        {
            QJsonObject rootObject = jsonDoc.object();

            QJsonObject::iterator it = rootObject.find(QString("ButtonDefinitions"));
            if (it != rootObject.end())
            {
                rootObject.erase(it);
            }

            QJsonObject buttonDefinitionsObject = QJsonObject();

            for (ScapeEngine::ButtonDefinitionDataAccessObject::ButtonDefinitions::const_iterator it =
                     buttonDefinitions.begin();
                 it != buttonDefinitions.end(); it++)
            {
                std::string currentButton =
                    ScapeEngine::ButtonId::getButtonIdMap().find(it->getButtonId())->second;

                QJsonObject buttonDefinitionDetailObject = QJsonObject();
                buttonDefinitionDetailObject.insert(QString("priority"),
                                                    QString::number(it->getPriority()));

                QJsonArray definitions;

                if (buttonDefinitionDetailObject.contains(QString("definitions")))
                {
                    definitions = buttonDefinitionDetailObject[QString("definitions")].toArray();
                }
                else
                {
                    definitions = QJsonArray();
                }

                QJsonObject::iterator itD = buttonDefinitionDetailObject.find(QString("definitions"));
                if (itD != rootObject.end())
                {
                    rootObject.erase(itD);
                }

                const ButtonDefinition::DeviceButtonSetsVector& deviceButtonSets =
                    it->getDeviceButtonSets();
                for (ScapeEngine::ButtonDefinition::DeviceButtonSetsVector::const_iterator
                         deviceButtonSetsIt = deviceButtonSets.begin();
                     deviceButtonSetsIt != deviceButtonSets.end(); deviceButtonSetsIt++)
                {
                    QJsonArray currentDeviceButtonArray = QJsonArray();
                    for (ScapeEngine::ButtonDefinition::DeviceButtonSet::const_iterator deviceButtonsIt =
                             deviceButtonSetsIt->begin();
                         deviceButtonsIt != deviceButtonSetsIt->end(); deviceButtonsIt++)
                    {
                        std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string> deviceButtons =
                            ScapeEngine::DeviceButtonId::getDeviceButtonIdMap();
                        std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string>::iterator
                            devIt = deviceButtons.find((*deviceButtonsIt));
                        if (devIt != deviceButtons.end())
                        {
                            currentDeviceButtonArray.append(QString(devIt->second.c_str()));
                        }
                    }
                    definitions.append(currentDeviceButtonArray);
                }

                buttonDefinitionDetailObject.insert(QString("definitions"), definitions);

                QJsonObject::iterator itB = buttonDefinitionsObject.find(QString(currentButton.c_str()));
                if (itB != rootObject.end())
                {
                    buttonDefinitionsObject.erase(itB);
                }

                buttonDefinitionsObject.insert(QString(QString(currentButton.c_str())),
                                               buttonDefinitionDetailObject);
            }

            rootObject.insert(QString("ButtonDefinitions"), buttonDefinitionsObject);

            jsonDoc = QJsonDocument(rootObject);
        }
        mFileHelper.writeJSONFile(jsonDoc);
    }
}
}
