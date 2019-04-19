#include "QtJSONHelper.h"

#include "ScapeEngineStableHeaders.h"

#include <QJsonObject>

namespace ScapeEngine
{

QtJSONHelper::QtJSONHelper() {}

QtJSONHelper::~QtJSONHelper() {}

QJsonDocument QtJSONHelper::parseJSON(const std::vector<char>& contents, QJsonParseError& error) const
{
    error.error = QJsonParseError::NoError;

    QJsonDocument jsonDoc;

    if (contents.size() > 0)
    {
        jsonDoc = QJsonDocument::fromJson(QByteArray(contents.data(), contents.size()), &error);
    }
    else
    {
        jsonDoc = QJsonDocument(QJsonObject());
    }

    if (error.error != QJsonParseError::NoError)
    {
        Ogre::LogManager::getSingleton().logMessage("QtJSONHelper: Error while reading JSON from data: " +
                                                    error.errorString().toStdString());
    }

    return jsonDoc;
}

std::vector<char> QtJSONHelper::renderJSON(const QJsonDocument& document) const
{
    QByteArray outData = document.toJson();
    return std::vector<char>(outData.data(), outData.data() + outData.size());
}
}
