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

namespace ScapeEngine
{
namespace ButtonDefinitionXMLSerializer
{
// Load button definitions from a file
void importSceneFromResource(const string& fileName, const string& groupName);

// Load button definitions from a memory buffer
void importXML(const string& rawData);
}
}

#endif // __BUTTONDEFINITIONXMLSERIALIZER_H__
