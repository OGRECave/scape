#include "PropertiesWidget.h"

#include <OgreString.h>
#include <OgreStringConverter.h>

PropertiesWidget::PropertiesWidget(QWidget* parent) : QtTreePropertyBrowser(parent), mPropertyManager(NULL)
{
    mPropertyManager = new QtVariantPropertyManager(this);
    connect(mPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                        this, SLOT(valueChanged(QtProperty *, const QVariant &)));
}

PropertiesWidget::~PropertiesWidget()
{
    delete mPropertyManager;
}

void PropertiesWidget::populate(const UIElementPropertyGridItemList& itemList,
                                const std::map<std::string, std::string>& valueMap)
{
    clear();
    mPropertyManager->clear();
    mPropertyToItem.clear();

    std::map<std::string, QtProperty*> categoryItems;

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

        QtProperty* parent = NULL;

        std::map<std::string, QtProperty*>::const_iterator categoryItemIt =
            categoryItems.find(categoryName);
        if (categoryItemIt == categoryItems.end())
        {
            QtProperty* topItem = mPropertyManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                                                QString(categoryName.c_str()));
            categoryItems[categoryName] = topItem;
            parent = topItem;
        }
        else
        {
            parent = categoryItemIt->second;
        }

        int qType;
        if (type == "INT")
        {
            qType = QVariant::Int;
        }
        else if (type == "REAL")
        {
            qType = QVariant::Double;
        }
        else if (type == "BOOL")
        {
            qType = QVariant::Bool;
        }
        else if (type == "COLOR")
        {
            qType = QVariant::Color;
        }
        /*else if (type == "FILE")
        {
            // This part of the PropertyBrowser doesn't work!
            qType = QtVariantPropertyManager::filePathTypeId();
        }*/
        else
        {
            qType = QVariant::String;
        }

        QtVariantProperty* item = mPropertyManager->addProperty(qType, QString(name.c_str()));
        mPropertyToItem[item] = *itemIt;

        std::map<std::string, std::string>::const_iterator pos = valueMap.find(itemIt->name);
        if (pos != valueMap.end())
        {
            std::string value = pos->second;
            item->setValue(QString(value.c_str()));
        }
        item->setToolTip(QString(itemIt->description.c_str()));

        parent->addSubProperty(item);
    }

    QtVariantEditorFactory* variantFactory = new QtVariantEditorFactory();
    setFactoryForManager(mPropertyManager, variantFactory);

    for (std::map<std::string, QtProperty*>::const_iterator it = categoryItems.begin();
         it != categoryItems.end(); it++)
    {
        addProperty(it->second);
    }
}

void PropertiesWidget::setValue(const std::string& key, const std::string& value)
{
    Ogre::String keyTrimmed = Ogre::String(key);
    Ogre::StringUtil::trim(keyTrimmed);
    for (std::map<QtProperty*, UIElementPropertyGridItem>::const_iterator it = mPropertyToItem.begin();
         it != mPropertyToItem.end(); it++)
    {
        Ogre::String curKeyTrimmed = Ogre::String(it->second.name);
        Ogre::StringUtil::trim(curKeyTrimmed);
        if (curKeyTrimmed == keyTrimmed)
        {
            ((QtVariantProperty*)it->first)->setValue(QString(value.c_str()));
        }
    }
}

void PropertiesWidget::valueChanged(QtProperty *property, const QVariant &value)
{
    std::map<QtProperty*, UIElementPropertyGridItem>::const_iterator propertyKeyIt =
        mPropertyToItem.find(property);
    if (propertyKeyIt != mPropertyToItem.end())
    {
        emit propertyValueChanged(propertyKeyIt->second.name, value.toString().toStdString());
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
