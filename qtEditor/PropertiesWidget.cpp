#include "PropertiesWidget.h"

#include <OgreString.h>
#include <OgreStringConverter.h>

#include <QList>

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
    mPropertiesWidgetUI = new Ui::PropertiesWidget();
    mPropertiesWidgetUI->setupUi(this);

    mItemModel = new QStandardItemModel();
    mItemModel->setColumnCount(2);
    mItemModel->setHeaderData(0, Qt::Horizontal, "Property");
    mItemModel->setHeaderData(1, Qt::Horizontal, "Value");

    mPropertiesWidgetUI->propertiesTreeView->setModel(mItemModel);

    connect(mItemModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
}

PropertiesWidget::~PropertiesWidget() { delete mPropertiesWidgetUI; }

void PropertiesWidget::populate(const UIElementPropertyGridItemList& itemList,
                                const std::map<std::string, std::string>& valueMap)
{
    mPropertyToItem.clear();

    mItemModel->clear();
    mItemModel->setColumnCount(2);
    mItemModel->setHeaderData(0, Qt::Horizontal, "Property");
    mItemModel->setHeaderData(1, Qt::Horizontal, "Value");

    QStandardItem* rootItem = mItemModel->invisibleRootItem();

    std::map<std::string, QStandardItem*> categoryItems;

    UIElementPropertyGridItemList::const_iterator itemIt, itemItEnd = itemList.end();
    for (itemIt = itemList.begin(); itemIt != itemItEnd; ++itemIt)
    {
        const std::string& name = itemIt->label;

        std::string type = itemIt->type;

        std::string categoryName = itemIt->category;
        if (categoryName.length() == 0)
        {
            categoryName = "General";
        }

        QStandardItem* parent = NULL;

        std::map<std::string, QStandardItem*>::const_iterator categoryItemIt =
            categoryItems.find(categoryName);
        if (categoryItemIt == categoryItems.end())
        {
            QList<QStandardItem*> categoryListItems;

            QStandardItem* nameItem = new QStandardItem();
            nameItem->setData(QString(categoryName.c_str()), Qt::DisplayRole);
            nameItem->setEditable(false);

            QStandardItem* dummyItem = new QStandardItem();
            dummyItem->setData(QString(), Qt::DisplayRole);
            dummyItem->setEditable(false);

            categoryListItems << nameItem;
            categoryListItems << dummyItem;

            rootItem->appendRow(categoryListItems);

            categoryItems[categoryName] = nameItem;
            parent = nameItem;
        }
        else
        {
            parent = categoryItemIt->second;
        }

        QVariant varValue = QVariant(QString());

        std::map<std::string, std::string>::const_iterator pos = valueMap.find(itemIt->name);
        if (pos != valueMap.end())
        {
            std::string value = pos->second;
            if (type == "COLOR")
            {
                QColor color = convertInternalColorToQColor(value);
                varValue = QVariant(color);
            }
            else
            {
                varValue = QVariant(QString(value.c_str()));
            }
        }

        QVariant::Type varType = QVariant::String;
        if (type == "INT")
        {
            varType = QVariant::Int;
        }
        else if (type == "REAL")
        {
            varType = QVariant::Double;
        }
        else if (type == "BOOL")
        {
            varType = QVariant::Bool;
        }

        if (!varValue.convert(varType))
        {
            varValue = QVariant(varType);
        }

        QList<QStandardItem*> listItems;

        QStandardItem* nameItem = new QStandardItem();
        nameItem->setData(QString(name.c_str()), Qt::DisplayRole);
        nameItem->setEditable(false);
        nameItem->setToolTip(QString(itemIt->description.c_str()));

        QStandardItem* valueItem = new QStandardItem();
        valueItem->setData(varValue, Qt::DisplayRole);
        valueItem->setEditable(true);

        listItems << nameItem;
        listItems << valueItem;

        parent->appendRow(listItems);

        mPropertyToItem.insert(std::make_pair(valueItem, *itemIt));
    }

    mPropertiesWidgetUI->propertiesTreeView->expandAll();
}

void PropertiesWidget::setValue(const std::string& key, const std::string& value)
{
    Ogre::String keyTrimmed = Ogre::String(key);
    Ogre::StringUtil::trim(keyTrimmed);
    for (std::map<QStandardItem*, UIElementPropertyGridItem>::const_iterator it = mPropertyToItem.begin();
         it != mPropertyToItem.end(); it++)
    {
        Ogre::String curKeyTrimmed = Ogre::String(it->second.name);
        Ogre::StringUtil::trim(curKeyTrimmed);
        if (curKeyTrimmed == keyTrimmed)
        {
            if (it->second.type == "COLOR")
            {
                QColor color = convertInternalColorToQColor(value.c_str());
                it->first->setData(QVariant(color), Qt::DisplayRole);
            }
            else
            {
                QVariant varValue = QVariant(QString(value.c_str()));
                QVariant::Type type = it->first->data(Qt::DisplayRole).type();
                if (!varValue.convert(type))
                {
                    varValue = QVariant(type);
                }

                it->first->setData(varValue, Qt::DisplayRole);
            }
        }
    }
}

void PropertiesWidget::itemChanged(QStandardItem* item)
{
    std::map<QStandardItem*, UIElementPropertyGridItem>::const_iterator propertyKeyIt =
        mPropertyToItem.find(item);
    if (propertyKeyIt != mPropertyToItem.end())
    {
        std::string valueStr;
        if (propertyKeyIt->second.type == "COLOR")
        {
            valueStr = convertQColorToInternalColor(item->data(Qt::DisplayRole).value<QColor>());
        }
        else
        {
            valueStr = item->data(Qt::DisplayRole).toString().toStdString();
        }
        emit propertyValueChanged(propertyKeyIt->second.name, valueStr);
    }
}

QColor PropertiesWidget::convertInternalColorToQColor(const std::string& internalColor)
{
    Ogre::String colorStr = Ogre::String(internalColor);
    colorStr = Ogre::StringUtil::replaceAll(colorStr, "(", "");
    colorStr = Ogre::StringUtil::replaceAll(colorStr, ")", "");
    std::vector<Ogre::String> colorParts = Ogre::StringUtil::split(colorStr, ",");
    QColor color;
    if (colorParts.size() == 3)
    {
        int redValue = Ogre::StringConverter::parseInt(colorParts[0], 0);
        int greenValue = Ogre::StringConverter::parseInt(colorParts[1], 0);
        int blueValue = Ogre::StringConverter::parseInt(colorParts[2], 0);
        color = QColor(redValue, greenValue, blueValue);
    }
    else
    {
        color = QColor();
    }
    return color;
}

std::string PropertiesWidget::convertQColorToInternalColor(const QColor qColor)
{
    Ogre::String redValue = Ogre::StringConverter::toString(qColor.red());
    Ogre::String greenValue = Ogre::StringConverter::toString(qColor.green());
    Ogre::String blueValue = Ogre::StringConverter::toString(qColor.blue());
    return "(" + redValue + "," + greenValue + "," + blueValue + ")";
}
