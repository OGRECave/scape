#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include <qttreepropertybrowser.h>
#include <string>
#include <list>

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
};

#endif // PROPERTIESWIDGET_H
