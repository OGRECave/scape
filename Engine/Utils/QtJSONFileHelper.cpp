#include "QtJSONFileHelper.h"

#include "ScapeEngineStableHeaders.h"

#include <QJsonObject>

namespace ScapeEngine
{

QtJSONFileHelper::QtJSONFileHelper(std::string fileName) : mFileName(fileName), mJSONHelper() {}

QtJSONFileHelper::~QtJSONFileHelper() {}

std::string QtJSONFileHelper::getFileName() const { return mFileName; }

QJsonDocument QtJSONFileHelper::readJSONFile(QJsonParseError& error) const
{
    Ogre::LogManager::getSingleton().logMessage("QtJSONFileHelper: Reading JSON from " + mFileName + "...");

    QJsonDocument ret = QJsonDocument(QJsonObject());

    std::ifstream fp;
    fp.open(mFileName.c_str(), std::ios::in | std::ios::binary);

    if (!fp)
    {
        Ogre::LogManager::getSingleton().logMessage("QtJSONFileHelper: Failed to open file " + mFileName);
    }
    else
    {
        std::vector<char> inBuf =
            std::vector<char>(std::istreambuf_iterator<char>(fp), std::istreambuf_iterator<char>());
        fp.close();

        ret = mJSONHelper.parseJSON(inBuf, error);

        if (error.error != QJsonParseError::NoError)
        {
            Ogre::LogManager::getSingleton().logMessage("QtJSONFileHelper: Error while reading JSON from " +
                                                        mFileName + ": " +
                                                        error.errorString().toStdString());
        }
    }

    return ret;
}

void QtJSONFileHelper::writeJSONFile(const QJsonDocument& document) const
{
    Ogre::LogManager::getSingleton().logMessage("QtJSONFileHelper: Writing JSON to " + mFileName + "...");

    std::ofstream ofp;
    ofp.open(mFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (ofp)
    {
        std::vector<char> outData = mJSONHelper.renderJSON(document);
        ofp.write(outData.data(), outData.size());

        ofp.close();
    }
    else
    {
        Ogre::LogManager::getSingleton().logMessage("QtJSONFileHelper: Failed to write to file " +
                                                    mFileName);
    }
}
}
