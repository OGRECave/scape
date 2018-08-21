/*/////////////////////////////////////////////////////////////////////////////////
/// An
///    ___   ____ _______
///   / _ \ / ___|_ ____/
///  | | | | |  _ | |__
///  | |_| | |_| || ___|
///   \___/ \____||_|
///                              File
///
/// Copyright (c) 2011 - 2015 Ismail TARIM <ismail@royalspor.com> and the Ogitor Team
//
//This program is free software; you can redistribute it and/or modify it under
//the terms of the GNU Lesser General Public License as published by the Free Software
//Foundation; either version 2 of the License, or (at your option) any later
//version.
//
//This program is distributed in the hope that it will be useful, but WITHOUT
//ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
//FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public License along with
//this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//Place - Suite 330, Boston, MA 02111-1307, USA, or go to
//http://www.gnu.org/copyleft/lesser.txt.
////////////////////////////////////////////////////////////////////////////////*/

#include "complexproperties.h"
#include "qtpropertymanager.h"
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QVBoxLayout>

//----------------------------------------------------------------------------------------
QuaternionManager::QuaternionManager(QObject* parent) : QtVariantPropertyManager(parent)
{
    connect(this, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this,
            SLOT(slotValueChanged(QtProperty*, const QVariant&)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty*)), this, SLOT(slotPropertyDestroyed(QtProperty*)));
}
//----------------------------------------------------------------------------------------
QuaternionManager::~QuaternionManager() {}
//----------------------------------------------------------------------------------------
void QuaternionManager::slotValueChanged(QtProperty* property, const QVariant& value)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveLeft(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveTop(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.setWidth(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (wToProperty.contains(property))
    {
        QtProperty* pointProperty = wToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.setHeight(value.value<double>());
        setValue(pointProperty, QVariant(p));
    }
}
//----------------------------------------------------------------------------------------
void QuaternionManager::slotPropertyDestroyed(QtProperty* property)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        propertyToData[pointProperty].x = 0;
        xToProperty.remove(property);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        propertyToData[pointProperty].y = 0;
        yToProperty.remove(property);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        propertyToData[pointProperty].z = 0;
        zToProperty.remove(property);
    }
    else if (wToProperty.contains(property))
    {
        QtProperty* pointProperty = wToProperty[property];
        propertyToData[pointProperty].w = 0;
        wToProperty.remove(property);
    }
}
//----------------------------------------------------------------------------------------
bool QuaternionManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}
//----------------------------------------------------------------------------------------
int QuaternionManager::valueType(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return QVariant::RectF;
    return QtVariantPropertyManager::valueType(propertyType);
}
//----------------------------------------------------------------------------------------
QVariant QuaternionManager::value(const QtProperty* property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}
//----------------------------------------------------------------------------------------
QString QuaternionManager::valueText(const QtProperty* property) const
{
    if (propertyToData.contains(property))
    {
        QVariant v = propertyToData[property].value;
        QRectF p = v.value<QRectF>();
        return QString("(%1;%2;%3;%4)")
            .arg(QString::number(p.height()))
            .arg(QString::number(p.x()))
            .arg(QString::number(p.y()))
            .arg(QString::number(p.width()));
    }
    return QtVariantPropertyManager::valueText(property);
}
//----------------------------------------------------------------------------------------
void QuaternionManager::setValue(QtProperty* property, const QVariant& val)
{
    if (propertyToData.contains(property))
    {
        if (val.type() != QVariant::RectF)
            return;
        QRectF p = val.value<QRectF>();
        Data d = propertyToData[property];
        d.value = p;
        if (d.x)
            d.x->setValue(p.x());
        if (d.y)
            d.y->setValue(p.y());
        if (d.z)
            d.z->setValue(p.width());
        if (d.w)
            d.w->setValue(p.height());
        propertyToData[property] = d;
        emit propertyChanged(property);
        emit valueChanged(property, p);
        return;
    }
    QtVariantPropertyManager::setValue(property, val);
}
//----------------------------------------------------------------------------------------
void QuaternionManager::initializeProperty(QtProperty* property)
{
    if (propertyType(property) == QVariant::RectF)
    {
        Data d;

        d.value = QRectF(0, 0, 0, 1);

        QuaternionManager* that = (QuaternionManager*)this;

        d.x = that->addProperty(QVariant::Double);
        d.x->setPropertyName("X");
        property->addSubProperty(d.x);
        xToProperty[d.x] = property;

        d.y = that->addProperty(QVariant::Double);
        d.y->setPropertyName("Y");
        property->addSubProperty(d.y);
        yToProperty[d.y] = property;

        d.z = that->addProperty(QVariant::Double);
        d.z->setPropertyName("Z");
        property->addSubProperty(d.z);
        zToProperty[d.z] = property;

        d.w = that->addProperty(QVariant::Double);
        d.w->setPropertyName("W");
        property->addSubProperty(d.w);
        wToProperty[d.w] = property;

        propertyToData[property] = d;
    }
    QtVariantPropertyManager::initializeProperty(property);
}
//----------------------------------------------------------------------------------------
void QuaternionManager::uninitializeProperty(QtProperty* property)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        if (d.x)
            xToProperty.remove(d.x);
        if (d.y)
            yToProperty.remove(d.y);
        if (d.z)
            zToProperty.remove(d.z);
        if (d.w)
            wToProperty.remove(d.w);
        propertyToData.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
Vector4Manager::Vector4Manager(QObject* parent) : QtVariantPropertyManager(parent)
{
    connect(this, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this,
            SLOT(slotValueChanged(QtProperty*, const QVariant&)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty*)), this, SLOT(slotPropertyDestroyed(QtProperty*)));
}
//----------------------------------------------------------------------------------------
Vector4Manager::~Vector4Manager() {}
//----------------------------------------------------------------------------------------
void Vector4Manager::slotValueChanged(QtProperty* property, const QVariant& value)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveLeft(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveTop(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.setWidth(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (wToProperty.contains(property))
    {
        QtProperty* pointProperty = wToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.setHeight(value.value<double>());
        setValue(pointProperty, QVariant(p));
    }
}
//----------------------------------------------------------------------------------------
void Vector4Manager::slotPropertyDestroyed(QtProperty* property)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        propertyToData[pointProperty].x = 0;
        xToProperty.remove(property);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        propertyToData[pointProperty].y = 0;
        yToProperty.remove(property);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        propertyToData[pointProperty].z = 0;
        zToProperty.remove(property);
    }
    else if (wToProperty.contains(property))
    {
        QtProperty* pointProperty = wToProperty[property];
        propertyToData[pointProperty].w = 0;
        wToProperty.remove(property);
    }
}
//----------------------------------------------------------------------------------------
bool Vector4Manager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}
//----------------------------------------------------------------------------------------
int Vector4Manager::valueType(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return QVariant::RectF;
    return QtVariantPropertyManager::valueType(propertyType);
}
//----------------------------------------------------------------------------------------
QVariant Vector4Manager::value(const QtProperty* property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}
//----------------------------------------------------------------------------------------
QString Vector4Manager::valueText(const QtProperty* property) const
{
    if (propertyToData.contains(property))
    {
        QVariant v = propertyToData[property].value;
        QRectF p = v.value<QRectF>();
        return QString("(%1;%2;%3;%4)")
            .arg(QString::number(p.x()))
            .arg(QString::number(p.y()))
            .arg(QString::number(p.width()))
            .arg(QString::number(p.height()));
    }
    return QtVariantPropertyManager::valueText(property);
}
//----------------------------------------------------------------------------------------
void Vector4Manager::setValue(QtProperty* property, const QVariant& val)
{
    if (propertyToData.contains(property))
    {
        if (val.type() != QVariant::RectF)
            return;
        QRectF p = val.value<QRectF>();
        Data d = propertyToData[property];
        d.value = p;
        if (d.x)
            d.x->setValue(p.x());
        if (d.y)
            d.y->setValue(p.y());
        if (d.z)
            d.z->setValue(p.width());
        if (d.w)
            d.w->setValue(p.height());
        if (d.x)
            d.x->setValue(p.x());
        propertyToData[property] = d;
        emit propertyChanged(property);
        emit valueChanged(property, p);
        return;
    }
    QtVariantPropertyManager::setValue(property, val);
}
//----------------------------------------------------------------------------------------
void Vector4Manager::setPropertyNames(QtProperty* property, QString fx, QString fy, QString fz, QString fw)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        d.x->setPropertyName(fx);
        d.y->setPropertyName(fy);
        d.z->setPropertyName(fz);
        d.w->setPropertyName(fw);
    }
}
//----------------------------------------------------------------------------------------
void Vector4Manager::initializeProperty(QtProperty* property)
{
    if (propertyType(property) == QVariant::RectF)
    {
        Data d;

        d.value = QRectF(0, 0, 0, 0);

        Vector4Manager* that = (Vector4Manager*)this;

        d.x = that->addProperty(QVariant::Double);
        d.x->setPropertyName("X");
        property->addSubProperty(d.x);
        xToProperty[d.x] = property;

        d.y = that->addProperty(QVariant::Double);
        d.y->setPropertyName("Y");
        property->addSubProperty(d.y);
        yToProperty[d.y] = property;

        d.z = that->addProperty(QVariant::Double);
        d.z->setPropertyName("Z");
        property->addSubProperty(d.z);
        zToProperty[d.z] = property;

        d.w = that->addProperty(QVariant::Double);
        d.w->setPropertyName("W");
        property->addSubProperty(d.w);
        wToProperty[d.w] = property;

        propertyToData[property] = d;
    }
    QtVariantPropertyManager::initializeProperty(property);
}
//----------------------------------------------------------------------------------------
void Vector4Manager::uninitializeProperty(QtProperty* property)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        if (d.x)
            xToProperty.remove(d.x);
        if (d.y)
            yToProperty.remove(d.y);
        if (d.z)
            zToProperty.remove(d.z);
        if (d.w)
            wToProperty.remove(d.w);
        propertyToData.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
Vector3Manager::Vector3Manager(QObject* parent) : QtVariantPropertyManager(parent)
{
    connect(this, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this,
            SLOT(slotValueChanged(QtProperty*, const QVariant&)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty*)), this, SLOT(slotPropertyDestroyed(QtProperty*)));
}
//----------------------------------------------------------------------------------------
Vector3Manager::~Vector3Manager() {}
//----------------------------------------------------------------------------------------
void Vector3Manager::slotValueChanged(QtProperty* property, const QVariant& value)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveLeft(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.moveTop(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        QVariant v = this->value(pointProperty);
        QRectF p = v.value<QRectF>();
        p.setWidth(value.value<double>());
        setValue(pointProperty, p);
    }
}
//----------------------------------------------------------------------------------------
void Vector3Manager::slotPropertyDestroyed(QtProperty* property)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        propertyToData[pointProperty].x = 0;
        xToProperty.remove(property);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        propertyToData[pointProperty].y = 0;
        yToProperty.remove(property);
    }
    else if (zToProperty.contains(property))
    {
        QtProperty* pointProperty = zToProperty[property];
        propertyToData[pointProperty].z = 0;
        zToProperty.remove(property);
    }
}
//----------------------------------------------------------------------------------------
bool Vector3Manager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}
//----------------------------------------------------------------------------------------
int Vector3Manager::valueType(int propertyType) const
{
    if (propertyType == QVariant::RectF)
        return QVariant::RectF;
    return QtVariantPropertyManager::valueType(propertyType);
}
//----------------------------------------------------------------------------------------
QVariant Vector3Manager::value(const QtProperty* property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}
//----------------------------------------------------------------------------------------
QString Vector3Manager::valueText(const QtProperty* property) const
{
    if (propertyToData.contains(property))
    {
        QVariant v = propertyToData[property].value;
        QRectF p = v.value<QRectF>();
        return QString("(%1;%2;%3)")
            .arg(QString::number(p.x()))
            .arg(QString::number(p.y()))
            .arg(QString::number(p.width()));
    }
    return QtVariantPropertyManager::valueText(property);
}
//----------------------------------------------------------------------------------------
void Vector3Manager::setValue(QtProperty* property, const QVariant& val)
{
    if (propertyToData.contains(property))
    {
        if (val.type() != QVariant::RectF)
            return;
        QRectF p = val.value<QRectF>();
        Data d = propertyToData[property];
        d.value = p;
        if (d.x)
            d.x->setValue(p.x());
        if (d.y)
            d.y->setValue(p.y());
        if (d.z)
            d.z->setValue(p.width());
        propertyToData[property] = d;
        emit propertyChanged(property);
        emit valueChanged(property, p);
        return;
    }
    QtVariantPropertyManager::setValue(property, val);
}
//----------------------------------------------------------------------------------------
void Vector3Manager::setPropertyNames(QtProperty* property, QString fx, QString fy, QString fz)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        d.x->setPropertyName(fx);
        d.y->setPropertyName(fy);
        d.z->setPropertyName(fz);
    }
}
//----------------------------------------------------------------------------------------
void Vector3Manager::initializeProperty(QtProperty* property)
{
    if (propertyType(property) == QVariant::RectF)
    {
        Data d;

        d.value = QRectF(0, 0, 0, 0);

        Vector3Manager* that = (Vector3Manager*)this;

        d.x = that->addProperty(QVariant::Double);
        d.x->setPropertyName("X");
        property->addSubProperty(d.x);
        xToProperty[d.x] = property;

        d.y = that->addProperty(QVariant::Double);
        d.y->setPropertyName("Y");
        property->addSubProperty(d.y);
        yToProperty[d.y] = property;

        d.z = that->addProperty(QVariant::Double);
        d.z->setPropertyName("Z");
        property->addSubProperty(d.z);
        zToProperty[d.z] = property;

        propertyToData[property] = d;
    }
    QtVariantPropertyManager::initializeProperty(property);
}
//----------------------------------------------------------------------------------------
void Vector3Manager::uninitializeProperty(QtProperty* property)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        if (d.x)
            xToProperty.remove(d.x);
        if (d.y)
            yToProperty.remove(d.y);
        if (d.z)
            zToProperty.remove(d.z);
        propertyToData.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
Vector2Manager::Vector2Manager(QObject* parent) : QtVariantPropertyManager(parent)
{
    connect(this, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this,
            SLOT(slotValueChanged(QtProperty*, const QVariant&)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty*)), this, SLOT(slotPropertyDestroyed(QtProperty*)));
}
//----------------------------------------------------------------------------------------
Vector2Manager::~Vector2Manager() {}
//----------------------------------------------------------------------------------------
void Vector2Manager::slotValueChanged(QtProperty* property, const QVariant& value)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        QVariant v = this->value(pointProperty);
        QPointF p = v.value<QPointF>();
        p.setX(value.value<double>());
        setValue(pointProperty, p);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        QVariant v = this->value(pointProperty);
        QPointF p = v.value<QPointF>();
        p.setY(value.value<double>());
        setValue(pointProperty, p);
    }
}
//----------------------------------------------------------------------------------------
void Vector2Manager::slotPropertyDestroyed(QtProperty* property)
{
    if (xToProperty.contains(property))
    {
        QtProperty* pointProperty = xToProperty[property];
        propertyToData[pointProperty].x = 0;
        xToProperty.remove(property);
    }
    else if (yToProperty.contains(property))
    {
        QtProperty* pointProperty = yToProperty[property];
        propertyToData[pointProperty].y = 0;
        yToProperty.remove(property);
    }
}
//----------------------------------------------------------------------------------------
bool Vector2Manager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::PointF)
        return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}
//----------------------------------------------------------------------------------------
int Vector2Manager::valueType(int propertyType) const
{
    if (propertyType == QVariant::PointF)
        return QVariant::PointF;
    return QtVariantPropertyManager::valueType(propertyType);
}
//----------------------------------------------------------------------------------------
QVariant Vector2Manager::value(const QtProperty* property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}
//----------------------------------------------------------------------------------------
QString Vector2Manager::valueText(const QtProperty* property) const
{
    if (propertyToData.contains(property))
    {
        QVariant v = propertyToData[property].value;
        QPointF p = v.value<QPointF>();
        return QString("(%1;%2)").arg(QString::number(p.x())).arg(QString::number(p.y()));
    }
    return QtVariantPropertyManager::valueText(property);
}
//----------------------------------------------------------------------------------------
void Vector2Manager::setValue(QtProperty* property, const QVariant& val)
{
    if (propertyToData.contains(property))
    {
        if (val.type() != QVariant::PointF)
            return;
        QPointF p = val.value<QPointF>();
        Data d = propertyToData[property];
        d.value = p;
        if (d.x)
            d.x->setValue(p.x());
        if (d.y)
            d.y->setValue(p.y());
        propertyToData[property] = d;
        emit propertyChanged(property);
        emit valueChanged(property, p);
        return;
    }
    QtVariantPropertyManager::setValue(property, val);
}
//----------------------------------------------------------------------------------------
void Vector2Manager::setPropertyNames(QtProperty* property, QString fx, QString fy)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        d.x->setPropertyName(fx);
        d.y->setPropertyName(fy);
    }
}
//----------------------------------------------------------------------------------------
void Vector2Manager::initializeProperty(QtProperty* property)
{
    if (propertyType(property) == QVariant::PointF)
    {
        Data d;

        d.value = QPointF(0, 0);

        Vector2Manager* that = (Vector2Manager*)this;

        d.x = that->addProperty(QVariant::Double);
        d.x->setPropertyName("X");
        property->addSubProperty(d.x);
        xToProperty[d.x] = property;

        d.y = that->addProperty(QVariant::Double);
        d.y->setPropertyName("Y");
        property->addSubProperty(d.y);
        yToProperty[d.y] = property;

        propertyToData[property] = d;
    }
    QtVariantPropertyManager::initializeProperty(property);
}
//----------------------------------------------------------------------------------------
void Vector2Manager::uninitializeProperty(QtProperty* property)
{
    if (propertyToData.contains(property))
    {
        Data d = propertyToData[property];
        if (d.x)
            xToProperty.remove(d.x);
        if (d.y)
            yToProperty.remove(d.y);
        propertyToData.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}
//----------------------------------------------------------------------------------------
