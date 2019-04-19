#ifndef __QTJSONHELPER_H__
#define __QTJSONHELPER_H__

#include "ScapeEnginePrerequisites.h"

#include <QJsonDocument>

namespace ScapeEngine
{
class QtJSONHelper
{
public:
    QtJSONHelper();
    virtual ~QtJSONHelper();

    virtual QJsonDocument parseJSON(const std::vector<char>& contents, QJsonParseError& error) const;
    virtual std::vector<char> renderJSON(const QJsonDocument& document) const;
};
}

#endif /* __QTJSONHELPER_H__ */
