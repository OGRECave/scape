/**
 * ScapeEngine::UIElementClasses
 *
 * Defines several base classes that describe different UI interface descriptions
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __UIELEMENTCLASSES__
#define __UIELEMENTCLASSES__

#include "ScapeEnginePrerequisites.h"

//#include "UIElementTypes.h"

namespace ScapeEngine
{
extern const string defaultPresetString;

// ----------------------------------------------------------------------------
template <class FactoryBaseClass> class FactoryClass
{
public:
    static FactoryBaseClass* create() { return NULL; }
    static const _TCHAR* getStaticClassName();
    virtual const _TCHAR* getClassName() = 0;
    virtual ~FactoryClass() {}
};

// ----------------------------------------------------------------------------
#define DEFINE_FACTORYCLASS(className, baseClassName)                                                      \
    static baseClassName* create() { return new className(); }                                             \
    static const _TCHAR* getStaticClassName() { return _T(#className); }                                   \
    virtual const _TCHAR* getClassName() { return _T(#className); }

// ----------------------------------------------------------------------------
#define DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(className)                                                   \
public:                                                                                                    \
    virtual const _TCHAR* getContainerName() { return _T(#className); }

// ----------------------------------------------------------------------------
#define IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(className)

// ----------------------------------------------------------------------------
#define LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS() loadPersistentProperties()

// ----------------------------------------------------------------------------
class UIElementContainer
{
public:
    virtual const _TCHAR* getContainerName() = 0;

    virtual StringList getUIElementPropertyFieldNameList() = 0;

    virtual string getUIElementPropertyFieldValue(const string& elementName) = 0;
    virtual string setUIElementPropertyValue(const string& elementName, const string& value) = 0;
    virtual StringStringMap getUIElementPropertyFieldValueMap() = 0;
    virtual StringStringMap setUIElementPropertyValueMap(const StringStringMap& valueMap) = 0;
};

// ----------------------------------------------------------------------------
class UIElementPresetContainer : virtual public UIElementContainer
{
public:
    virtual string getUIElementPresetPropertyValue(const string& presetName, const string& elementName) = 0;
    virtual void setUIElementPresetPropertyValue(const string& presetName, const string& elementName,
                                                 const string& value) = 0;
    virtual StringStringMap getUIElementPresetPropertyValueMap(const string& presetName) = 0;
    virtual void setUIElementPresetPropertyValueMap(const string& presetName,
                                                    const StringStringMap& valueMap) = 0;
    virtual void deleteUIElementPreset(const string& presetName) = 0;

    virtual StringList getUIElementPresetPropertyNames() = 0;
    virtual string importUIElementPreset(const string& fileName) = 0;
    virtual void exportUIElementPreset(const string& fileName) = 0;

    virtual string makeUniquePresetName(const string& baseName) = 0;
};

// ----------------------------------------------------------------------------
class UIElementContainerSimple : virtual public UIElementContainer
{
public:
    virtual StringList getUIElementPropertyFieldNameList();
    virtual string getUIElementPropertyFieldValue(const string& elementName);
    virtual StringStringMap getUIElementPropertyFieldValueMap() { return getPersistentElementValueMap(); }
    virtual StringStringMap setUIElementPropertyValueMap(const StringStringMap& valueMap);

    virtual string setUIElementPropertyValue(const string& elementName, const string& value) = 0;

protected:
    virtual StringEnumMap& getPersistentElementStringEnumMap();
    virtual StringStringMap& getPersistentElementValueMap();

    virtual bool initPersistentElementValueMap(StringStringMap& map) = 0;
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map) = 0;

    virtual void loadPersistentProperties();
};

// ----------------------------------------------------------------------------
class UIElementPresetContainerSimple : public UIElementContainerSimple, public UIElementPresetContainer
{
public:
    virtual string getUIElementPresetPropertyValue(const string& presetName, const string& elementName);
    virtual void setUIElementPresetPropertyValue(const string& presetName, const string& elementName,
                                                 const string& value);
    virtual StringStringMap getUIElementPresetPropertyValueMap(const string& presetName);
    virtual void setUIElementPresetPropertyValueMap(const string& presetName,
                                                    const StringStringMap& valueMap);
    virtual void deleteUIElementPreset(const string& presetName);

    virtual StringList getUIElementPresetPropertyNames();
    virtual string importUIElementPreset(const string& fileName);
    virtual void exportUIElementPreset(const string& fileName);

    virtual string makeUniquePresetName(const string& baseName);
};
}

#endif // __UIELEMENTCLASSES__
