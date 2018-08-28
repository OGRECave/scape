#include "propertieswidget.h"

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
    mPropertyToKey.clear();

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
        else if (type == "FILE")
        {
            qType = QtVariantPropertyManager::filePathTypeId();
        }
        else
        {
            qType = QVariant::String;
        }

        QtVariantProperty* item = mPropertyManager->addProperty(qType, QString(name.c_str()));
        mPropertyToKey[item] = itemIt->name;

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

void PropertiesWidget::valueChanged(QtProperty *property, const QVariant &value)
{
    std::map<QtProperty *, std::string>::const_iterator propertyKeyIt = mPropertyToKey.find(property);
    if (propertyKeyIt != mPropertyToKey.end())
    {
        emit propertyValueChanged(propertyKeyIt->second, value.toString().toStdString());
    }
}
