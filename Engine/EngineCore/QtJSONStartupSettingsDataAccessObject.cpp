#include "ScapeEngineStableHeaders.h"
#include "QtJSONStartupSettingsDataAccessObject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace ScapeEngine
{

QtJSONStartupSettingsDataAccessObject::QtJSONStartupSettingsDataAccessObject(std::string fileName)
    : mFileHelper(fileName)
{
}

QtJSONStartupSettingsDataAccessObject::~QtJSONStartupSettingsDataAccessObject() {}

const StartupSettings QtJSONStartupSettingsDataAccessObject::getStartupSettings() const
{
    StartupSettings ret;

    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc = mFileHelper.readJSONFile(errorStruct);
    if (errorStruct.error == QJsonParseError::NoError)
    {
        if (jsonDoc.isObject())
        {
            QJsonObject rootObject = jsonDoc.object();

            if (rootObject.contains(QString("Startup")))
            {
                QJsonValue startupValue = rootObject[QString("Startup")];
                if (startupValue.isObject())
                {
                    QJsonObject startupObject = startupValue.toObject();
                    if (startupObject.contains(QString("Heightfield")))
                    {
                        QJsonValue heightfieldValue = startupObject[QString("Heightfield")];
                        if (heightfieldValue.isObject())
                        {
                            QJsonObject heightfieldObject = heightfieldValue.toObject();
                            if (heightfieldObject.contains(QString("rows")))
                            {
                                QJsonValue heightfieldRowsValue = heightfieldObject[QString("rows")];
                                if (heightfieldRowsValue.isDouble())
                                {
                                    ret.setHeightfieldRows(heightfieldRowsValue.toInt(0));
                                }
                            }

                            if (heightfieldObject.contains(QString("columns")))
                            {
                                QJsonValue heightfieldColumnsValue = heightfieldObject[QString("columns")];
                                if (heightfieldColumnsValue.isDouble())
                                {
                                    ret.setHeightfieldColumns(heightfieldColumnsValue.toInt(0));
                                }
                            }

                            if (heightfieldObject.contains(QString("height")))
                            {
                                QJsonValue heightfieldHeightValue = heightfieldObject[QString("height")];
                                if (heightfieldHeightValue.isDouble())
                                {
                                    ret.setHeightfieldHeight(heightfieldHeightValue.toDouble(0));
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

void QtJSONStartupSettingsDataAccessObject::updateStartupSettings(const StartupSettings& startupSettings)
{
    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc = mFileHelper.readJSONFile(errorStruct);

    if (errorStruct.error == QJsonParseError::NoError)
    {
        if (jsonDoc.isObject())
        {
            QJsonObject rootObject = jsonDoc.object();

            QJsonObject::iterator it = rootObject.find(QString("Startup"));
            if (it != rootObject.end())
            {
                rootObject.erase(it);
            }

            QJsonObject startupObject = QJsonObject();

            QJsonObject heightfieldObject = QJsonObject();

            heightfieldObject.insert(QString("rows"), startupSettings.getHeightfieldRows());

            heightfieldObject.insert(QString("columns"), startupSettings.getHeightfieldColumns());

            heightfieldObject.insert(QString("height"), startupSettings.getHeightfieldHeight());

            startupObject.insert(QString("Heightfield"), heightfieldObject);

            rootObject.insert(QString("Startup"), startupObject);

            jsonDoc = QJsonDocument(rootObject);
        }
        mFileHelper.writeJSONFile(jsonDoc);
    }
}
}
