/**
 * ScapeEngine::HeightfieldSerializer namespace
 *
 * Functions for incremental serialization and previous/next reconstruction deserialization
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDSERIALIZERINCREMENTAL_H__
#define __HEIGHTFIELDSERIALIZERINCREMENTAL_H__

#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
class HeightfieldBuffer;

namespace HeightfieldSerializer
{
Utils::SerialMemoryBufferPtr serializeIncremental(HeightfieldBuffer* inPreviousBuffer,
                                                  HeightfieldBuffer* inNextBuffer);
bool deserializeIncrementalNext(Utils::SerialMemoryBufferConstPtr inSerial, HeightfieldBuffer* buffer,
                                string* error = NULL);
bool deserializeIncrementalPrevious(Utils::SerialMemoryBufferConstPtr inSerial, HeightfieldBuffer* buffer,
                                    string* error = NULL);
}
}

#endif // __HEIGHTFIELDSERIALIZERINCREMENTAL_H__