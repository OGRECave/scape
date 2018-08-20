/**
 * ScapeEngine::BMPHeader header structs
 *
 * BMP file format header structures
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __BMPHEADER_H__
#define __BMPHEADER_H__

namespace ScapeEngine
{
	namespace Utils
	{
		// Bitmap compression types.
		enum EBitmapCompression
		{
			BITMAPCOMPRESSION_RGB       = 0,
			BITMAPCOMPRESSION_RLE8      = 1,
			BITMAPCOMPRESSION_RLE4      = 2,
			BITMAPCOMPRESSION_BITFIELDS = 3,
		};

		// .BMP file header.
		#pragma pack(push,1)
		struct BitmapFileHeader
		{
			Ogre::uint16 type;
			Ogre::uint32 size;
			Ogre::uint16 reserved1;
			Ogre::uint16 reserved2;
			Ogre::uint32 offBits;

			void flipByteOrder()
			{
				Utils::flipByteOrder(&type);
				Utils::flipByteOrder(&size);
				Utils::flipByteOrder(&reserved1);
				Utils::flipByteOrder(&reserved2);
				Utils::flipByteOrder(&offBits);
			}
		};
		#pragma pack(pop)

		// .BMP subheader.
		#pragma pack(push,1)
		struct BitmapInfoHeader
		{
			Ogre::uint32 size;
			Ogre::uint32 width;
			Ogre::uint32 height;
			Ogre::uint16 planes;
			Ogre::uint16 bitCount;
			Ogre::uint32 compression;
			Ogre::uint32 sizeImage;
			Ogre::uint32 xPelsPerMeter;
			Ogre::uint32 yPelsPerMeter;
			Ogre::uint32 clrUsed;
			Ogre::uint32 clrImportant;

			void flipByteOrder()
			{
				Utils::flipByteOrder(&size);
				Utils::flipByteOrder(&width);
				Utils::flipByteOrder(&height);
				Utils::flipByteOrder(&planes);
				Utils::flipByteOrder(&bitCount);
				Utils::flipByteOrder(&compression);
				Utils::flipByteOrder(&sizeImage);
				Utils::flipByteOrder(&xPelsPerMeter);
				Utils::flipByteOrder(&yPelsPerMeter);
				Utils::flipByteOrder(&clrUsed);
				Utils::flipByteOrder(&clrImportant);
			}

		};
		#pragma pack(pop)
	}
}

#endif // __BMPHEADER_H__