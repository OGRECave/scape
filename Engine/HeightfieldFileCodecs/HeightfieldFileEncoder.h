/**
 * ScapeEngine::HeightfieldFileEncoder base class
 *
 * Interface for heightfield file export
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDFILEENCODER_H__
#define __HEIGHTFIELDFILEENCODER_H__

#include "EngineCore/UIElementClasses.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	class HeightfieldBuffer;

	// ----------------------------------------------------------------------------
	class HeightfieldFileEncoder : public UIElementContainerSimple, public FactoryClass<HeightfieldFileEncoder>
	{
	public:

		virtual const string& getFileExtension() = 0;
		virtual const string& getFileFilter() = 0;

		virtual bool encode(HeightfieldBuffer *inBuffer, const string& fileName, string *error = NULL) = 0;
	};

}

#endif // __HEIGHTFIELDFILEENCODER_H__