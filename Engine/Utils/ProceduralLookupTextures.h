/**
 * ScapeEngine::ProceduralLookupTextures class
 *
 * Image creation of perlin permutation and gradient tables
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __PROCEDURALLOOKUPTEXTURES_H__
#define __PROCEDURALLOOKUPTEXTURES_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{
namespace Utils
{
void createPerlinPerm2DTexture(const string& textureName);
void createPerlinGrad2DTexture(const string& textureName);
}
}

#endif // __PROCEDURALLOOKUPTEXTURES_H__