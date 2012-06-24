/**
 * ScapeEngine::HeightfieldSerializer namespace
 *
 * Simple full serializer/deserializer for a heightfieldbuffer
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDSERIALIZER_H__
#define __HEIGHTFIELDSERIALIZER_H__

#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
	class HeightfieldBuffer;

	namespace HeightfieldSerializer
	{
		Utils::SerialMemoryBufferPtr serialize(HeightfieldBuffer *inBuffer, Ogre::PixelFormat pixelFormat);
		bool deserialize(Utils::SerialMemoryBufferConstPtr inSerial, HeightfieldBuffer* outBuffer, Ogre::PixelFormat pixelFormat, string *error = NULL);
	}
}

#endif // __HEIGHTFIELDSERIALIZER_H__