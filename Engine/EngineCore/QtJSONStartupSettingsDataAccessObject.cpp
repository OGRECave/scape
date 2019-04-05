#include "ScapeEngineStableHeaders.h"
#include "QtJSONStartupSettingsDataAccessObject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace ScapeEngine
{

QtJSONStartupSettingsDataAccessObject::QtJSONStartupSettingsDataAccessObject(std::string fileName)
    : mFileName(fileName)
{
}

QtJSONStartupSettingsDataAccessObject::~QtJSONStartupSettingsDataAccessObject() {}

const StartupSettings QtJSONStartupSettingsDataAccessObject::getStartupSettings() const
{
    Ogre::LogManager::getSingleton().logMessage(
        "QtJSONStartupSettingsDataAccessObject: Reading JSON from " + mFileName + "...");

    StartupSettings ret;

    std::ifstream fp;
    fp.open(mFileName.c_str(), std::ios::in | std::ios::binary);

    if (!fp)
    {
        Ogre::LogManager::getSingleton().logMessage(
            "QtJSONSettingsDatasetDataAccessObject: Failed to open file " + mFileName);
    }
    else
    {
        std::vector<char> inBuf =
            std::vector<char>(std::istreambuf_iterator<char>(fp), std::istreambuf_iterator<char>());
        fp.close();

        QJsonParseError errorStruct;
        errorStruct.error = QJsonParseError::NoError;

        QJsonDocument jsonDoc =
            QJsonDocument::fromJson(QByteArray(inBuf.data(), inBuf.size()), &errorStruct);
        if (errorStruct.error != QJsonParseError::NoError)
        {
            Ogre::LogManager::getSingleton().logMessage(
                "QtJSONSettingsDatasetDataAccessObject: Error while reading JSON from " + mFileName + ": " +
                errorStruct.errorString().toStdString());
        }
        else
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
                                    QJsonValue heightfieldColumnsValue =
                                        heightfieldObject[QString("columns")];
                                    if (heightfieldColumnsValue.isDouble())
                                    {
                                        ret.setHeightfieldColumns(heightfieldColumnsValue.toInt(0));
                                    }
                                }

                                if (heightfieldObject.contains(QString("height")))
                                {
                                    QJsonValue heightfieldHeightValue =
                                        heightfieldObject[QString("height")];
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
    }

    return ret;
}

void QtJSONStartupSettingsDataAccessObject::updateStartupSettings(const StartupSettings& startupSettings)
{
    Ogre::LogManager::getSingleton().logMessage("QtJSONStartupSettingsDataAccessObject: Writing JSON to " +
                                                mFileName + "...");

    std::ifstream ifp;
    ifp.open(mFileName.c_str(), std::ios::in | std::ios::binary);

    std::vector<char> inBuf;

    if (ifp)
    {
        inBuf = std::vector<char>(std::istreambuf_iterator<char>(ifp), std::istreambuf_iterator<char>());
        ifp.close();
    }

    QJsonParseError errorStruct;
    errorStruct.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc;

    if (inBuf.size() > 0)
    {
        jsonDoc = QJsonDocument::fromJson(QByteArray(inBuf.data(), inBuf.size()), &errorStruct);
    }
    else
    {
        jsonDoc = QJsonDocument(QJsonObject());
    }

    if (errorStruct.error != QJsonParseError::NoError)
    {
        Ogre::LogManager::getSingleton().logMessage(
            "QtJSONStartupSettingsDataAccessObject: Error while reading JSON from " + mFileName + ": " +
            errorStruct.errorString().toStdString());
    }
    else
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
    }

    std::ofstream ofp;
    ofp.open(mFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (ofp)
    {
        QByteArray outData = jsonDoc.toJson();
        ofp.write(outData.data(), outData.size());

        ofp.close();
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage(
            "QtJSONStartupSettingsDataAccessObject: Failed to write to file " + mFileName);
    }
}
}
