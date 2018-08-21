/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldFileDecoderRaw.h"
#include "Utils/SerialMemoryBuffer.h"


using namespace ScapeEngine;

#define UIELEMENTPROPERTY_FLIPX		_T("FLIPX")
#define UIELEMENTPROPERTY_FLIPY		_T("FLIPY")
#define UIELEMENTPROPERTY_BIGENDIAN	_T("BIGENDIAN")
#define UIELEMENTPROPERTY_BPP		_T("BPP")
#define UIELEMENTPROPERTY_WIDTH		_T("WIDTH")
#define UIELEMENTPROPERTY_HEIGHT	_T("HEIGHT")

enum EPropertyId
{
	PROPERTYID_FLIPX,
	PROPERTYID_FLIPY,
	PROPERTYID_BIGENDIAN,
	PROPERTYID_BPP,
	PROPERTYID_WIDTH,
	PROPERTYID_HEIGHT,
};

// ----------------------------------------------------------------------------
HeightfieldFileDecoderRaw::HeightfieldFileDecoderRaw()
:	mCodecExtension(_T("RAW")), mFlipX(false), mFlipY(false) 
{
	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldFileDecoderRaw)

const string& HeightfieldFileDecoderRaw::getFileExtension() {static string fileExtension(_T("RAW")); return fileExtension;}
const string& HeightfieldFileDecoderRaw::getFileFilter() {static string fileFilter(_T("*.RAW;*.R8;*.R16;*.R32")); return fileFilter;}


// ----------------------------------------------------------------------------
bool HeightfieldFileDecoderRaw::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPX, _T("0"));
	setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPY, _T("0"));
	setUIElementPropertyValue(UIELEMENTPROPERTY_BIGENDIAN, _T("0"));
	setUIElementPropertyValue(UIELEMENTPROPERTY_BPP, _T("0"));
	setUIElementPropertyValue(UIELEMENTPROPERTY_WIDTH, _T("0"));
	setUIElementPropertyValue(UIELEMENTPROPERTY_HEIGHT, _T("0"));
	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldFileDecoderRaw::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	map[UIELEMENTPROPERTY_FLIPX] = PROPERTYID_FLIPX;
	map[UIELEMENTPROPERTY_FLIPY] = PROPERTYID_FLIPY;
	map[UIELEMENTPROPERTY_BIGENDIAN] = PROPERTYID_BIGENDIAN;
	map[UIELEMENTPROPERTY_BPP] = PROPERTYID_BPP;
	map[UIELEMENTPROPERTY_WIDTH] = PROPERTYID_WIDTH;
	map[UIELEMENTPROPERTY_HEIGHT] = PROPERTYID_HEIGHT;

	return true;
}

// ----------------------------------------------------------------------------
string HeightfieldFileDecoderRaw::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	bool boolValue = Ogre::StringConverter::parseBool(value);
	int intValue = Ogre::StringConverter::parseInt(value);
	string outValue = Utils::emptyString;

	if (elementName.compare(UIELEMENTPROPERTY_FLIPX) == 0) {mFlipX = boolValue; outValue = Ogre::StringConverter::toString(boolValue);}
	else if (elementName.compare(UIELEMENTPROPERTY_FLIPY) == 0) {mFlipY = boolValue; outValue = Ogre::StringConverter::toString(boolValue);}
	else if (elementName.compare(UIELEMENTPROPERTY_BIGENDIAN) == 0) {mBigEndian = boolValue; outValue = Ogre::StringConverter::toString(boolValue);}
	else if (elementName.compare(UIELEMENTPROPERTY_BPP) == 0) {mBpp = intValue; outValue = Ogre::StringConverter::toString(intValue);}
	else if (elementName.compare(UIELEMENTPROPERTY_WIDTH) == 0) {mWidth = intValue; outValue = Ogre::StringConverter::toString(intValue);}
	else if (elementName.compare(UIELEMENTPROPERTY_HEIGHT) == 0) {mHeight = intValue; outValue = Ogre::StringConverter::toString(intValue);}

	getPersistentElementValueMap()[elementName] = outValue;
	return outValue;
}


// ----------------------------------------------------------------------------
bool HeightfieldFileDecoderRaw::decode(
	HeightfieldBuffer *outBuffer, 
	const string& fileName, 
	string* error)
{
	Ogre::LogManager::getSingleton().logMessage(_T("Importing heightfield '") + outBuffer->getName() + _T("' from file '") + fileName + _T("' using decoder '") + getClassName() + _T("'"));
	assert(outBuffer);

	Ogre::PixelFormat pixelFormat = Ogre::PF_UNKNOWN;
	switch (mBpp)
	{
	case 0: pixelFormat = Ogre::PF_BYTE_A; break;
	case 1: pixelFormat = Ogre::PF_SHORT_L; break;
	case 2: pixelFormat = Ogre::PF_FLOAT32_R; break;
	default: {assert(false && _T("Unknown RAW format in HeightfieldFileDecoderRaw::decode()")); break;}
	}

	if (pixelFormat != Ogre::PF_UNKNOWN)
	{
		std::pair<void*, size_t> fileData = Utils::readBinaryFile(fileName);
		if (fileData.first)
		{
			size_t byteCount = Ogre::PixelUtil::getMemorySize(mWidth, mHeight, 1, pixelFormat);
			if (byteCount <= fileData.second)
			{
				Ogre::PixelBox rawPixelBox(mWidth, mHeight, 1, pixelFormat, fileData.first);

				decode(&rawPixelBox, mFlipX, mFlipY, mBigEndian, NULL);

				outBuffer->updateFrom(rawPixelBox);

				delete fileData.first;

				return true;
			}
			else if (error)
			{
				*error = _T("File too small for specified size and bitdepth");
			}
		}
		else if (error)
		{
			*error = _T("File could not be opened");
		}
	} 
	else if (error)
	{
		*error = _T("Specified bitdepth not supported");
	}
	return false;
}


// ----------------------------------------------------------------------------
bool HeightfieldFileDecoderRaw::decode(
	Ogre::PixelBox* pixelBox, 
	bool flipX, 
	bool flipY, 
	bool bigEndian, 
	string *error)
{
	size_t lines = pixelBox->getHeight();
	size_t pixelByteCount = Ogre::PixelUtil::getNumElemBytes(pixelBox->format);
	size_t rowBytePitch = (int)pixelBox->rowPitch * pixelByteCount;

	bool flipEndian = ((OGRE_ENDIAN == OGRE_ENDIAN_BIG) != bigEndian) && pixelByteCount > 1;

	if (flipY)
	{
		for (size_t line = 0; line < lines / 2; ++line)
		{
			Ogre::uint8* rowData = (Ogre::uint8*)pixelBox->data + line * rowBytePitch;
			Ogre::uint8* altData = (Ogre::uint8*)pixelBox->data + (lines - 1 - line) * rowBytePitch;
			for (Ogre::uint8* data = rowData; data < rowData + rowBytePitch; ++data, ++altData)
			{
				unsigned char tmp = *data;
				*data = *altData;
				*altData = tmp;
			}
		}
	}

	for (size_t line = 0; line < lines; ++line)
	{
		Ogre::uint8* rowData = (Ogre::uint8*)pixelBox->data + line * rowBytePitch;
		if (flipX)
		{
			if (flipEndian) 
			{
				Utils::flipByteOrder(rowData, rowBytePitch);
			}
			else
			{
				Utils::flipSuperWordOrder(rowData, pixelBox->rowPitch, pixelByteCount);
			}
		}
		else if (flipEndian) 
		{
			Utils::flipSubWordOrder(rowData, pixelBox->rowPitch, pixelByteCount);
		}
	}

	return true;
}
