/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ProceduralLookupTextures.h"
#include "PCH/stdafx.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
// Original Ken Perlin's permutation table
static Ogre::uint8 perlinPermTable[256] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,
    69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,
    94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
    60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161,
    255, 216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
    255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213,
    119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193,
    238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
    181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180};

// ----------------------------------------------------------------------------
// 2D off-axis gradients table
// (128*(1+cos(angle)), 128*(1+sin(angle)) pairs for
// eight angles: 22.5, 67.5, 112.5, 157.5, 202.5, 247.5, 292.5, 337.5
static Ogre::uint8 perlinGradTable[16] = {245, 176, 176, 245, 79,  245, 10,  176,
                                          10,  79,  79,  10,  176, 10,  245, 79};

// ----------------------------------------------------------------------------
void Utils::createPerlinPerm2DTexture(const string& textureName)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Creating PerlinPerm2DTexture '") + textureName +
                                                _T("'..."));

    Ogre::uint32 perlinPerm2DTable[256 * 256];
    Ogre::uint32* ptr = &perlinPerm2DTable[0];

    // create optimized noise permutation table
    // r = perm(perm(x    ) + (y))
    // g = perm(perm(x    ) + (y + 1))
    // b = perm(perm(x + 1) + (y))
    // a = perm(perm(x + 1) + (y + 1))

    for (int y = 0; y < 256; ++y)
    {
        for (int x = 0; x < 256; ++x)
        {
            Ogre::uint8 a = perlinPermTable[x] + (Ogre::uint8)y;
            Ogre::uint8 b = perlinPermTable[(x + 1) & 255] + (Ogre::uint8)y;
            *ptr++ = (perlinPermTable[a] << 24) + (perlinPermTable[(a + 1) & 255] << 16) +
                     (perlinPermTable[b] << 8) + (perlinPermTable[(b + 1) & 255]);
        }
    }

    Ogre::Image perlinPerm2DImage;
    perlinPerm2DImage.loadDynamicImage((Ogre::uint8*)perlinPerm2DTable, 256, 256, 1, Ogre::PF_R8G8B8A8);

    Ogre::TextureManager::getSingleton().getByName(textureName)->loadImage(perlinPerm2DImage);

    perlinPerm2DImage.save(_T("PerlinPerm2D.png"));
}

// ----------------------------------------------------------------------------
void Utils::createPerlinGrad2DTexture(const string& textureName)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Creating PerlinGrad2DTexture '") + textureName +
                                                _T("'..."));

    Ogre::uint32 perlinGrad2DTable[256 * 256];
    Ogre::uint32* ptr = &perlinGrad2DTable[0];

    // create optimized noise gradient-permutation table
    // r = grad(perm(x) & 7).x
    // g = grad(perm(x) & 7).y
    // b = grad(perm(y) & 7).x
    // a = grad(perm(y) & 7).y
    for (int y = 0; y < 256; ++y)
    {
        for (int x = 0; x < 256; ++x)
        {
            Ogre::uint8 px = perlinPermTable[x];
            Ogre::uint8 py = perlinPermTable[y];
            *ptr++ = (perlinGradTable[((px & 7) << 1)] << 24) +
                     (perlinGradTable[((px & 7) << 1) + 1] << 16) +
                     (perlinGradTable[((py & 7) << 1)] << 8) + (perlinGradTable[((py & 7) << 1) + 1]);
        }
    }

    Ogre::Image perlinGrad2DImage;
    perlinGrad2DImage.loadDynamicImage((Ogre::uint8*)perlinGrad2DTable, 256, 256, 1, Ogre::PF_R8G8B8A8);

    Ogre::TextureManager::getSingleton().getByName(textureName)->loadImage(perlinGrad2DImage);

    perlinGrad2DImage.save(_T("PerlinGrad2D.png"));
}
