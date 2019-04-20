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
