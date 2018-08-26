#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>
#include <string>
#include <list>
#include <map>

struct UIElementPropertyGridItem
{
    std::string type;
    std::string name;
    std::string label;
    std::string description;
    std::string category;
};

typedef std::list<UIElementPropertyGridItem> UIElementPropertyGridItemList;

class PropertiesWidget : public QtTreePropertyBrowser
{
    Q_OBJECT;

public:
    PropertiesWidget(QWidget* parent = NULL);
    virtual ~PropertiesWidget();

    void populate(const UIElementPropertyGridItemList& itemList,
                  const std::map<std::string, std::string>& valueMap);

private:
    QtVariantPropertyManager* mPropertyManager;
    std::map<QtProperty*, std::string> mPropertyToKey;
};

#endif // PROPERTIESWIDGET_H
