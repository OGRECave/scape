#ifndef __QTJSONFILEHELPER_H__
#define __QTJSONFILEHELPER_H__

#include "ScapeEnginePrerequisites.h"

#include <QJsonDocument>

#include "QtJSONHelper.h"

namespace ScapeEngine
{

class QtJSONFileHelper
{
public:
    QtJSONFileHelper(std::string fileName);
    virtual ~QtJSONFileHelper();

    std::string getFileName() const;

    virtual QJsonDocument readJSONFile(QJsonParseError& error) const;
    virtual void writeJSONFile(const QJsonDocument& document) const;

protected:
    std::string mFileName;
    QtJSONHelper mJSONHelper;
};
}

#endif /* __QTJSONFILEHELPER_H__ */
