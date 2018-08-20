#include "propertieswidget.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "complexproperties.h"

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
    propertyBrowser = new GeneralTreeBrowser(this);
}

PropertiesWidget::~PropertiesWidget()
{
}

void PropertiesWidget::boolValueChanged(QtProperty* property, bool val)
{
}

void PropertiesWidget::stringValueChanged(QtProperty* property, const QString& val)
{
}

void PropertiesWidget::intValueChanged(QtProperty* property, int val)
{
}

void PropertiesWidget::colourValueChanged(QtProperty* property, const QColor& val)
{
}

void PropertiesWidget::doubleValueChanged(QtProperty* property, double val)
{
}

void PropertiesWidget::enumValueChanged(QtProperty* property, int val)
{
}

void PropertiesWidget::quaternionValueChanged(QtProperty* property, const QVariant& val)
{
}

void PropertiesWidget::vector2ValueChanged(QtProperty* property, const QVariant& val)
{
}

void PropertiesWidget::vector3ValueChanged(QtProperty* property, const QVariant& val)
{
}

void PropertiesWidget::vector4ValueChanged(QtProperty* property, const QVariant& val)
{
}
