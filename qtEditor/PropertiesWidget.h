#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>

#include "ui_propertieswidget.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
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

class PropertiesWidget : public QWidget
{
    Q_OBJECT;

public:
    PropertiesWidget(QWidget* parent = NULL);
    virtual ~PropertiesWidget();

    void populate(const UIElementPropertyGridItemList& itemList,
                  const std::map<std::string, std::string>& valueMap);

    void setValue(const std::string& key, const std::string& value);

private slots:
    void itemChanged(QStandardItem* item);

signals:
    void propertyValueChanged(const std::string& key, const std::string& value);

protected:
    static QColor convertInternalColorToQColor(const std::string& internalColor);
    static std::string convertQColorToInternalColor(const QColor qColor);

private:
    Ui::PropertiesWidget* mPropertiesWidgetUI;

    QStandardItemModel* mItemModel;
    std::map<QStandardItem*, UIElementPropertyGridItem> mPropertyToItem;
};

#endif // PROPERTIESWIDGET_H
