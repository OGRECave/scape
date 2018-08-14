/**
 * ScapeEngine::HeightfieldFileDecoder base class
 *
 * Interface for heightfield file import
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDFILEDECODER_H__
#define __HEIGHTFIELDFILEDECODER_H__

#include "EngineCore/UIElementClasses.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	class HeightfieldBuffer;

	// ----------------------------------------------------------------------------
	class HeightfieldFileDecoder : public UIElementContainerSimple, public FactoryClass<HeightfieldFileDecoder>
	{
	public:

		virtual const string& getFileExtension() = 0;
		virtual const string& getFileFilter() = 0;

		virtual bool decode(HeightfieldBuffer *inBuffer, const string& fileName, string *error = NULL) = 0;
	};

}

#endif // __HEIGHTFIELDFILEDECODER_H__