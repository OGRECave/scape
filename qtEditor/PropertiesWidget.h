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

    void setValue(const std::string& key, const std::string& value);

private slots:
    void valueChanged(QtProperty * property, const QVariant & value);

signals:
    void propertyValueChanged(const std::string& key, const std::string& value);

protected:
    static QColor convertInternalColorToQColor(const std::string& internalColor);
    static std::string convertQColorToInternalColor(const QColor qColor);

private:
    QtVariantPropertyManager* mPropertyManager;
    std::map<QtProperty*, UIElementPropertyGridItem> mPropertyToItem;
};

#endif // PROPERTIESWIDGET_H
