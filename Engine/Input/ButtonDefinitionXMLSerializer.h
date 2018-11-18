/**
 * ScapeEngine::ButtonDefinitionXML namespace
 *
 * Button definition XML file loading functionality
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __BUTTONDEFINITIONXMLSERIALIZER_H__
#define __BUTTONDEFINITIONXMLSERIALIZER_H__

#include "ButtonDefinitionDataAccessObject.h"

namespace ScapeEngine
{

class ButtonDefinitionXMLSerializer : public ButtonDefinitionDataAccessObject
{
public:
    ButtonDefinitionXMLSerializer(std::string fileName, std::string resourceGroupName);
    virtual ~ButtonDefinitionXMLSerializer();

    virtual const ButtonDefinitions getButtonDefinitions() const;

protected:
    // Load button definitions from a memory buffer
    const ButtonDefinitions importXML(const string& rawData) const;

    static void _readElementBUTTON(const TiXmlElement& buttonElem, ButtonDefinitions& definitions);
    static void _readElementDEF(const TiXmlElement& buttonDefinitionElem, ButtonDefinitions& definitions,
                                ButtonId::EButtonId buttonId, int priority);
    static void _readElementDBUTTON(const TiXmlElement& deviceButtonElem,
                                    ButtonDefinition& buttonDefinition);

    std::string mFileName;
    std::string mResourceGroupName;
};
}

#endif // __BUTTONDEFINITIONXMLSERIALIZER_H__
