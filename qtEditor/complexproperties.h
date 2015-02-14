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
#ifndef COMPLEXPROPERTIES_H
#define COMPLEXPROPERTIES_H

#include <QtVariantPropertyManager>
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"


class QuaternionManager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    QuaternionManager(QObject *parent = 0);
    ~QuaternionManager();

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QString valueText(const QtProperty *property) const;

public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private Q_SLOTS:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
private:
    struct Data {
        QVariant value;
        QtVariantProperty *w;
        QtVariantProperty *x;
        QtVariantProperty *y;
        QtVariantProperty *z;
    };
    QMap<const QtProperty *, Data> propertyToData;
    QMap<const QtProperty *, QtProperty *> wToProperty;
    QMap<const QtProperty *, QtProperty *> xToProperty;
    QMap<const QtProperty *, QtProperty *> yToProperty;
    QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector4Manager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    Vector4Manager(QObject *parent = 0);
    ~Vector4Manager();

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QString valueText(const QtProperty *property) const;

    void setPropertyNames(QtProperty *property, QString fx, QString fy, QString fz, QString fw);

public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private Q_SLOTS:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
private:
    struct Data {
        QVariant value;
        QtVariantProperty *w;
        QtVariantProperty *x;
        QtVariantProperty *y;
        QtVariantProperty *z;
    };
    QMap<const QtProperty *, Data> propertyToData;
    QMap<const QtProperty *, QtProperty *> wToProperty;
    QMap<const QtProperty *, QtProperty *> xToProperty;
    QMap<const QtProperty *, QtProperty *> yToProperty;
    QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector3Manager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    Vector3Manager(QObject *parent = 0);
    ~Vector3Manager();

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QString valueText(const QtProperty *property) const;

    void setPropertyNames(QtProperty *property, QString fx, QString fy, QString fz);

public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private Q_SLOTS:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
private:
    struct Data {
        QVariant value;
        QtVariantProperty *x;
        QtVariantProperty *y;
        QtVariantProperty *z;
    };
    QMap<const QtProperty *, Data> propertyToData;
    QMap<const QtProperty *, QtProperty *> xToProperty;
    QMap<const QtProperty *, QtProperty *> yToProperty;
    QMap<const QtProperty *, QtProperty *> zToProperty;
};

class Vector2Manager : public QtVariantPropertyManager
{
    Q_OBJECT
public:
    Vector2Manager(QObject *parent = 0);
    ~Vector2Manager();

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QString valueText(const QtProperty *property) const;

    void setPropertyNames(QtProperty *property, QString fx, QString fy);

public Q_SLOTS:
    virtual void setValue(QtProperty *property, const QVariant &val);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private Q_SLOTS:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
private:
    struct Data {
        QVariant value;
        QtVariantProperty *x;
        QtVariantProperty *y;
    };
    QMap<const QtProperty *, Data> propertyToData;
    QMap<const QtProperty *, QtProperty *> xToProperty;
    QMap<const QtProperty *, QtProperty *> yToProperty;
};

#endif // COMPLEXPROPERTIES_H
