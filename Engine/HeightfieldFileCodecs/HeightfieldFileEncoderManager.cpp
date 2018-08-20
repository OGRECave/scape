/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldFileEncoderManager.h"
#include "HeightfieldFileEncoder.h"
#include "HeightfieldFileEncoderImage.h"
#include "HeightfieldFileEncoderRaw.h"
#include "HeightfieldFileEncoderUnrealBMPG16.h"


using namespace ScapeEngine;


// ----------------------------------------------------------------------------
HeightfieldFileEncoderManager::HeightfieldFileEncoderManager()
{
	// 8-bit uint formats
	registerEncoder(new HeightfieldFileEncoderImageBMP8U);
	registerEncoder(new HeightfieldFileEncoderImagePGM8U);
	registerEncoder(new HeightfieldFileEncoderImagePNG8U);
	registerEncoder(new HeightfieldFileEncoderImageTGA8U);
	registerEncoder(new HeightfieldFileEncoderImageTIFF8U);
	registerEncoder(new HeightfieldFileEncoderRaw8);

	// 16-bit uint formats
	registerEncoder(new HeightfieldFileEncoderImagePGM16U);
	registerEncoder(new HeightfieldFileEncoderImagePNG16U);
	registerEncoder(new HeightfieldFileEncoderImageTIFF16U);
	registerEncoder(new HeightfieldFileEncoderRaw16L);
	registerEncoder(new HeightfieldFileEncoderRaw16B);
	registerEncoder(new HeightfieldFileEncoderUnrealBMPG16);

	// 32-bit flat formats
	registerEncoder(new HeightfieldFileEncoderImageTIFF32F);
	registerEncoder(new HeightfieldFileEncoderRaw32F);

}


// ----------------------------------------------------------------------------
HeightfieldFileEncoderManager::~HeightfieldFileEncoderManager()
{
	EncoderMap::iterator encoderIt, encoderItEnd = mEncoderMap.end();
	for (encoderIt = mEncoderMap.begin(); encoderIt != encoderItEnd; ++encoderIt)
	{
		delete encoderIt->second;
	}
}


// ----------------------------------------------------------------------------
StringList HeightfieldFileEncoderManager::getEncoderNameList()
{
	StringList list;

	EncoderMap::iterator encoderIt, encoderItEnd = mEncoderMap.end();
	for (encoderIt = mEncoderMap.begin(); encoderIt != encoderItEnd; ++encoderIt)
	{
		list.push_back(encoderIt->first);
	}

	return list;
}


// ----------------------------------------------------------------------------
StringStringStringPairMap HeightfieldFileEncoderManager::getFileFilterMap()
{
	return mFileFilterMap;
}


// ----------------------------------------------------------------------------
HeightfieldFileEncoder* HeightfieldFileEncoderManager::getEncoder(const string& name)
{
	HeightfieldFileEncoder* encoder = NULL;

	EncoderMap::iterator encoderIt = mEncoderMap.find(name);
	if (encoderIt != mEncoderMap.end())
	{
		encoder = encoderIt->second;
	}

	return encoder;
}


// ----------------------------------------------------------------------------
void HeightfieldFileEncoderManager::registerEncoder(HeightfieldFileEncoder* encoder)
{
	assert(mEncoderMap.find(encoder->getClassName()) == mEncoderMap.end());

	mEncoderMap[encoder->getClassName()] = encoder;
	
	mFileFilterMap[encoder->getClassName()] = std::pair<string, string>(encoder->getFileExtension(), encoder->getFileFilter());
}