/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

#include "HeightfieldOperationCPULevel.h"

using namespace ScapeEngine;

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPULevel);


// ----------------------------------------------------------------------------
enum EPropertyId
{
	PROPERTYID_STRENGTH,
	PROPERTYID_PENCILMODE,
	PROPERTYID_PATHSPACING,
};


// ----------------------------------------------------------------------------
bool HeightfieldOperationCPULevel::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString)); 
	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationCPULevel::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
	ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
	ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);

	return true;
}


// ----------------------------------------------------------------------------
string HeightfieldOperationCPULevel::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	EPropertyId propertyId = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
	int intValue = Ogre::StringConverter::parseInt(value);
	bool boolValue = Ogre::StringConverter::parseBool(value);
	string outValue;

	switch (propertyId)
	{
	case PROPERTYID_STRENGTH: {intValue = Utils::clamp(intValue, 0, 1000); mStrength = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PENCILMODE: {mPencilMode = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}
	case PROPERTYID_PATHSPACING: {intValue = Utils::clamp(intValue, 0, 1000); mPathSpacing = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	default: {Ogre::LogManager::getSingleton().logMessage(string(getClassName()) + _T("::setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'")); break;}
	}

	getPersistentElementValueMap()[elementName] = outValue;
	return outValue;
}


// ----------------------------------------------------------------------------
HeightfieldOperationCPULevel::HeightfieldOperationCPULevel()
:	HeightfieldOperationCPU()
{
//	mBrushName = "HeightfieldOperationCPULevel";
	mBrushName = _T("HeightfieldBrush");

	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}


// ----------------------------------------------------------------------------
void HeightfieldOperationCPULevel::applyPrimary(const Ogre::Vector3& position, Ogre::Real strength)
{
	assert(mBrush);
	HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

	Ogre::Real changeSpeed = 0.1f * mStrength;
	Ogre::Real sizeChangeScale = strength * changeSpeed;// * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
	Ogre::Real innerRadius = mBrush->getInnerRadius();
	Ogre::Real outerRadius = mBrush->getOuterRadius();
	int size = (int)(2*outerRadius) + 2;
	int left = (int)position.x;
	int top = (int)position.z;
	Ogre::Rect editRect(
		std::max(0,left-size/2), 
		std::max(0,top-size/2), 
		std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size/2),
		std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size/2));
	Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

	Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
	heightfieldBuffer->copyTo(pixelBox);
	Ogre::Real scale = 1.0f / (innerRadius*innerRadius-outerRadius*outerRadius);
	Ogre::Real bias = -outerRadius * outerRadius * scale;

	//Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() - heightfieldBuffer->getHeightRangeMin();
	Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
	//Ogre::Real cy = position.y;
	Ogre::Real cz = position.z - (Ogre::Real)editRect.top;


	Ogre::Real avgHeight = 0.0f;
	Ogre::Real totWeight = 0.0f;
	for (int x = 0; x < (int)editRect.width(); ++x)
	{
		for (int z = 0; z < (int)editRect.height(); ++z)
		{
			Ogre::Real hy = data[z*editRect.width()+x];
			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dz = (Ogre::Real)z - cz;

			Ogre::Real s = dx * dx + dz * dz;
			Ogre::Real weight = Utils::clamp(s * scale + bias, 0.0f, 1.0f);

			totWeight += weight;
			avgHeight += hy * weight;
		}
	}
	avgHeight /= totWeight;

	for (int x = 0; x < (int)editRect.width(); ++x)
	{
		for (int z = 0; z < (int)editRect.height(); ++z)
		{
			Ogre::Real hy = data[z*editRect.width()+x];
			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dz = (Ogre::Real)z - cz;

			Ogre::Real s = dx * dx + dz * dz;
			Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
			delta = delta * (avgHeight - hy);
			data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);

		}
	}
	heightfieldBuffer->updateFrom(pixelBox);
	heightfieldBuffer->revalidate();
	delete data;
}



// ----------------------------------------------------------------------------
void HeightfieldOperationCPULevel::applySecondary(const Ogre::Vector3& position, Ogre::Real strength)
{
	assert(mBrush);
	HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

	Ogre::Real changeSpeed = 0.1f * mStrength;
	Ogre::Real sizeChangeScale = strength * changeSpeed;// * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
	Ogre::Real innerRadius = mBrush->getInnerRadius();
	Ogre::Real outerRadius = mBrush->getOuterRadius();
	int size = (int)(2*outerRadius) + 2;
	int left = (int)position.x;
	int top = (int)position.z;
	Ogre::Rect editRect(
		std::max(0,left-size/2), 
		std::max(0,top-size/2), 
		std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size/2),
		std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size/2));
	Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

	Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
	heightfieldBuffer->copyTo(pixelBox);
	Ogre::Real scale = 1.0f / (innerRadius*innerRadius-outerRadius*outerRadius);
	Ogre::Real bias = -outerRadius * outerRadius * scale;

	//Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() - heightfieldBuffer->getHeightRangeMin();
	Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
	//Ogre::Real cy = position.y;
	Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

	Ogre::Real avgHeight = 0.0f;
	Ogre::Real totWeight = 0.0f;
	for (int x = 0; x < (int)editRect.width(); ++x)
	{
		for (int z = 0; z < (int)editRect.height(); ++z)
		{
			Ogre::Real hy = data[z*editRect.width()+x];
			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dz = (Ogre::Real)z - cz;

			Ogre::Real s = dx * dx + dz * dz;
			Ogre::Real weight = Utils::clamp(s * scale + bias, 0.0f, 1.0f);

			totWeight += weight;
			avgHeight += hy * weight;
		}
	}
	avgHeight /= totWeight;

	for (int x = 0; x < (int)editRect.width(); ++x)
	{
		for (int z = 0; z < (int)editRect.height(); ++z)
		{
			Ogre::Real hy = data[z*editRect.width()+x];
			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dz = (Ogre::Real)z - cz;

			Ogre::Real s = dx * dx + dz * dz;
			Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
			delta = delta * (hy - avgHeight);
			data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);

		}
	}
	heightfieldBuffer->updateFrom(pixelBox);
	heightfieldBuffer->revalidate();
	delete data;
}
