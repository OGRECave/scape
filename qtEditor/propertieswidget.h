#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include "qttreepropertybrowser.h"
#include <string>
#include <list>

class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtBoolPropertyManager;
class QtIntPropertyManager;
class QtDoublePropertyManager;
class QtStringPropertyManager;
class QtEnumPropertyManager;
class QtColorPropertyManager;
class QuaternionManager;
class Vector2Manager;
class Vector3Manager;
class Vector4Manager;
class QtProperty;

struct UIElementPropertyGridItem
{
    std::string type;
    std::string name;
    std::string label;
    std::string description;
    std::string category;
};

typedef std::list<UIElementPropertyGridItem> UIElementPropertyGridItemList;

class GeneralTreeBrowser: public QtTreePropertyBrowser
{
    Q_OBJECT;
public:
    GeneralTreeBrowser(QWidget* parent = 0) : QtTreePropertyBrowser(parent) {}
    virtual ~GeneralTreeBrowser() {}
};

class PropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertiesWidget(QWidget *parent = 0);
    ~PropertiesWidget();
    GeneralTreeBrowser* getPropertyBrowser() {return propertyBrowser;}

signals:

public slots:
    void boolValueChanged(QtProperty *property, bool val);
    void stringValueChanged(QtProperty *property, const QString& val);
    void intValueChanged(QtProperty *property, int val);
    void colourValueChanged(QtProperty *property, const QColor& val);
    void doubleValueChanged(QtProperty *property, double val);
    void enumValueChanged(QtProperty *property, int val);
    void quaternionValueChanged(QtProperty *property, const QVariant& val);
    void vector2ValueChanged(QtProperty *property, const QVariant& val);
    void vector3ValueChanged(QtProperty *property, const QVariant& val);
    void vector4ValueChanged(QtProperty *property, const QVariant& val);

protected:
    GeneralTreeBrowser* propertyBrowser;
    QtGroupPropertyManager* groupManager;
    QtBoolPropertyManager* boolManager;
    QtColorPropertyManager* colourManager;
    QtIntPropertyManager* intManager;
    QtDoublePropertyManager* doubleManager;
    QtStringPropertyManager* stringManager;
    QtEnumPropertyManager* enumManager;
    QuaternionManager* quaternionManager;
    Vector2Manager* vector2Manager;
    Vector3Manager* vector3Manager;
    Vector4Manager* vector4Manager;

};

#endif // PROPERTIESWIDGET_H
