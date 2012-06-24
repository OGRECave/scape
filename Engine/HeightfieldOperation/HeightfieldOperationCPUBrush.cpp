/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationCPUBrush.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "HeightfieldOperationTaskDirect.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

using namespace ScapeEngine;

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUBrush);


// ----------------------------------------------------------------------------
class HeightfieldOperationDirectCPUBrush : public HeightfieldOperationTaskDirect
{
public:
	// ----------------------------------------------------------------------------
	HeightfieldOperationDirectCPUBrush(HeightfieldOperationBrush* operation, const BrushInstance& brushInstance) 
	:	HeightfieldOperationTaskDirect(operation, brushInstance) {}

	// ----------------------------------------------------------------------------
	virtual void tickPending() {mCurrentState = STATE_ACTIVE;}

	// ----------------------------------------------------------------------------
	virtual void tickActive() {
		//assert(mBrush);
		HeightfieldOperationCPUBrush* operation = dynamic_cast<HeightfieldOperationCPUBrush*>(mOperation);
		HeightfieldBuffer* heightfieldBuffer = operation->getInOutHeightfieldBuffer();


		//Ogre::Real changeSpeed = mStrength * 0.01f;
		Ogre::Real sizeChangeScale = mBrushInstance.strength * mOperation->getStrength();// * 0.01f;//* changeSpeed;// * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
		Ogre::Real innerRadius = mBrushInstance.innerRadius;
		Ogre::Real outerRadius = mBrushInstance.outerRadius;
		int size = (int)(2*outerRadius) + 2;
		int left = (int)mBrushInstance.position.x;
		int top = (int)mBrushInstance.position.z;
		Ogre::Rect editRect(
			std::max(0,left-size/2), 
			std::max(0,top-size/2), 
			std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size/2),
			std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size/2));
		Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

		Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
		heightfieldBuffer->copyTo(pixelBox);

		//float power = 1.0f;//0.25f;
		//Ogre::Real scale = 1.0f / ((innerRadius)-(outerRadius));
		//Ogre::Real bias = -(outerRadius) * scale;
		Ogre::Real scale = 1.0f / ((innerRadius*innerRadius)-(outerRadius*outerRadius));
		Ogre::Real bias = -(outerRadius*outerRadius) * scale;

		Ogre::Real heightscale = heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMax() - heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMin();
		Ogre::Real cx = mBrushInstance.position.x - (Ogre::Real)editRect.left;
		Ogre::Real cy = mBrushInstance.position.y;
		Ogre::Real cz = mBrushInstance.position.z - (Ogre::Real)editRect.top;

		if (operation->isLedgeMode())
		{
			for (int x = 0; x < (int)editRect.width(); ++x)
			{
				for (int z = 0; z < (int)editRect.height(); ++z)
				{
					Ogre::Real hy = data[z*editRect.width()+x];
					Ogre::Real dx = (Ogre::Real)x - cx;
					Ogre::Real dy = hy * heightscale - cy;
					Ogre::Real dz = (Ogre::Real)z - cz;

					Ogre::Real s = dx * dx + dz * dz;
					Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
					delta = delta * Utils::clamp(dy * dy * scale + bias, 0.0f, 1.0f); // ledge
					data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);
				}
			}
		}
		else
		{
			for (int x = 0; x < (int)editRect.width(); ++x)
			{
				for (int z = 0; z < (int)editRect.height(); ++z)
				{
					Ogre::Real hy = data[z*editRect.width()+x];
					Ogre::Real dx = (Ogre::Real)x - cx;
					Ogre::Real dy = hy * heightscale - cy;
					Ogre::Real dz = (Ogre::Real)z - cz;

					//Ogre::Real s = powf(dx * dx + dz * dz, 0.5f);
					//Ogre::Real delta = Utils::clamp(s * scale + bias, 0.0f, 1.0f);
					//delta = sizeChangeScale * delta * delta;

					Ogre::Real s = dx * dx + dz * dz;
					Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
					delta = delta * Utils::clamp(1.0f - dy / innerRadius, 0.0f, 1.0f); //tunneler
					data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);
				}
			}
		}
		heightfieldBuffer->updateFrom(pixelBox);
		delete data;

		mUpdatedRect = Utils::unionTRect(mUpdatedRect, editRect);

		mCurrentState = STATE_COMPLETED;
	}

};


// ----------------------------------------------------------------------------
enum EPropertyId
{
	PROPERTYID_STRENGTH,
	PROPERTYID_PENCILMODE,
	PROPERTYID_PATHSPACING,
	PROPERTYID_LEDGEMODE,
};


// ----------------------------------------------------------------------------
bool HeightfieldOperationCPUBrush::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString)); 
	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationCPUBrush::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
	ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
	ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
	ADD_STRINGENUM(map, PROPERTYID_, LEDGEMODE);

	return true;
}


// ----------------------------------------------------------------------------
string HeightfieldOperationCPUBrush::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	EPropertyId propertyId = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
	int intValue = Ogre::StringConverter::parseInt(value);
	bool boolValue = Ogre::StringConverter::parseBool(value);
	string outValue;

	switch (propertyId)
	{
	case PROPERTYID_STRENGTH: {intValue = Utils::clamp(intValue, 0, 1000); mStrength = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PATHSPACING: {intValue = Utils::clamp(intValue, 0, 1000); mPathSpacing = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PENCILMODE: {mPencilMode = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}
	case PROPERTYID_LEDGEMODE: {mLedgeMode = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}
	default: {Ogre::LogManager::getSingleton().logMessage(string(getClassName()) + _T("::setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'")); break;}
	}

	getPersistentElementValueMap()[elementName] = outValue;
	return outValue;
}


// ----------------------------------------------------------------------------
HeightfieldOperationCPUBrush::HeightfieldOperationCPUBrush()
:	HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationCPUBrush, HeightfieldOperationPolicySchedulerTaskDirect<HeightfieldOperationCPUBrush, HeightfieldOperationBrushBase> >()
{
//	mBrushName = _T("HeightfieldOperationCPUBrush");
	mBrushName = _T("HeightfieldBrush");

	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskDirect* HeightfieldOperationCPUBrush::createTaskDirect(const BrushInstance& brushInstance)
{
	return new HeightfieldOperationDirectCPUBrush(this, brushInstance);
}


#ifdef __BLA__

// ----------------------------------------------------------------------------
void HeightfieldOperationCPUBrush::applyPrimary(const Ogre::Vector3& position, Ogre::Real strength)
{
	assert(mBrush);
	HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

	Ogre::Real changeSpeed = mStrength * 0.01f;
	Ogre::Real sizeChangeScale = strength * changeSpeed;// * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
	Ogre::Real innerRadius = mBrush->getInnerRadius();
	Ogre::Real outerRadius = mBrush->getOuterRadius();
	int size = (int)(2*outerRadius) + 2;
	int left = (int)position.x;
	int top = (int)position.z;
	Ogre::Rect editRect(
		std::max(0,left-size/2), 
		std::max(0,top-size/2), 
		std::min(heightfieldBuffer->getElementColumnCount(), left + size/2),
		std::min(heightfieldBuffer->getElementRowCount(), top + size/2));
	Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

	Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
	heightfieldBuffer->copyTo(pixelBox);

	//float power = 1.0f;//0.25f;
	//Ogre::Real scale = 1.0f / ((innerRadius)-(outerRadius));
	//Ogre::Real bias = -(outerRadius) * scale;
	Ogre::Real scale = 1.0f / ((innerRadius*innerRadius)-(outerRadius*outerRadius));
	Ogre::Real bias = -(outerRadius*outerRadius) * scale;

	Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() - heightfieldBuffer->getHeightRangeMin();
	Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
	Ogre::Real cy = position.y;
	Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

	if (mLedgeMode)
	{
		for (int x = 0; x < (int)editRect.width(); ++x)
		{
			for (int z = 0; z < (int)editRect.height(); ++z)
			{
				Ogre::Real hy = data[z*editRect.width()+x];
				Ogre::Real dx = (Ogre::Real)x - cx;
				Ogre::Real dy = hy * heightscale - cy;
				Ogre::Real dz = (Ogre::Real)z - cz;

				Ogre::Real s = dx * dx + dz * dz;
				Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
				delta = delta * Utils::clamp(dy * dy * scale + bias, 0.0f, 1.0f); // ledge
				data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);
			}
		}
	}
	else
	{
		for (int x = 0; x < (int)editRect.width(); ++x)
		{
			for (int z = 0; z < (int)editRect.height(); ++z)
			{
				Ogre::Real hy = data[z*editRect.width()+x];
				Ogre::Real dx = (Ogre::Real)x - cx;
				Ogre::Real dy = hy * heightscale - cy;
				Ogre::Real dz = (Ogre::Real)z - cz;

				//Ogre::Real s = powf(dx * dx + dz * dz, 0.5f);
				//Ogre::Real delta = Utils::clamp(s * scale + bias, 0.0f, 1.0f);
				//delta = sizeChangeScale * delta * delta;

				Ogre::Real s = dx * dx + dz * dz;
				Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
				delta = delta * Utils::clamp(1.0f - dy / innerRadius, 0.0f, 1.0f); //tunneler
				data[z*editRect.width()+x] = Utils::clamp(hy + delta, 0.0f, 1.0f);
			}
		}
	}
	heightfieldBuffer->updateFrom(pixelBox);
	heightfieldBuffer->revalidate();
	delete data;
}



// ----------------------------------------------------------------------------
void HeightfieldOperationCPUBrush::applySecondary(const Ogre::Vector3& position, Ogre::Real strength)
{
	assert(mBrush);
	HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

	Ogre::Real changeSpeed = mStrength * 0.01f;
	Ogre::Real sizeChangeScale = strength * changeSpeed;// * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
	Ogre::Real innerRadius = mBrush->getInnerRadius();
	Ogre::Real outerRadius = mBrush->getOuterRadius();
	int size = (int)(2*outerRadius) + 2;
	int left = (int)position.x;
	int top = (int)position.z;
	Ogre::Rect editRect(
		std::max(0,left-size/2), 
		std::max(0,top-size/2), 
		std::min(heightfieldBuffer->getElementColumnCount(), left + size/2),
		std::min(heightfieldBuffer->getElementRowCount(), top + size/2));
	Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

	Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
	heightfieldBuffer->copyTo(pixelBox);
  	Ogre::Real scale = 1.0f / (innerRadius*innerRadius-outerRadius*outerRadius);
  	Ogre::Real bias = -outerRadius * outerRadius * scale;

	Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() - heightfieldBuffer->getHeightRangeMin();
	Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
	Ogre::Real cy = position.y;
	Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

	if (mLedgeMode)
	{
		for (int x = 0; x < (int)editRect.width(); ++x)
		{
			for (int z = 0; z < (int)editRect.height(); ++z)
			{
				Ogre::Real hy = data[z*editRect.width()+x];
				Ogre::Real dx = (Ogre::Real)x - cx;
				Ogre::Real dy = hy * heightscale - cy;
				Ogre::Real dz = (Ogre::Real)z - cz;

				Ogre::Real s = dx * dx + dz * dz;
				Ogre::Real delta = sizeChangeScale * powf(Utils::clamp(s * scale + bias, 0.0f, 1.0f), 8.0f);
				delta = delta * Utils::clamp(dy * dy * scale + bias, 0.0f, 1.0f); // ledge
				data[z*editRect.width()+x] = Utils::clamp(hy - delta, 0.0f, 1.0f);

			}
		}
	}
	else
	{
		for (int x = 0; x < (int)editRect.width(); ++x)
		{
			for (int z = 0; z < (int)editRect.height(); ++z)
			{
				Ogre::Real hy = data[z*editRect.width()+x];
				Ogre::Real dx = (Ogre::Real)x - cx;
				Ogre::Real dy = hy * heightscale - cy;
				Ogre::Real dz = (Ogre::Real)z - cz;

  				Ogre::Real s = dx * dx + dz * dz;
  				Ogre::Real delta = sizeChangeScale * powf(Utils::clamp(s * scale + bias, 0.0f, 1.0f), 8.0f);
  				delta = delta * Utils::clamp(1.0f + dy / innerRadius, 0.0f, 1.0f) ; // tunneler
  				//delta = delta * Utils::clamp(1.0f - dy / innerRadius, 0.0f, 1.0f); //tunneler
				data[z*editRect.width()+x] = Utils::clamp(hy - delta, 0.0f, 1.0f);
			}
		}
	}
	heightfieldBuffer->updateFrom(pixelBox);
	heightfieldBuffer->revalidate();
	delete data;
}

// ----------------------------------------------------------------------------
void HeightfieldOperationCPUBrush::applySecondary(const Ogre::Vector3& position, Ogre::Real strength)
{
	HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

	Ogre::Real changeSpeed = 1.0f;
//	Ogre::Real changeSpeed = 10.0f;
	Ogre::Real sizeChangeScale = strength * changeSpeed;// * -(1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
	Ogre::Real innerRadius = mBrush->getInnerRadius();
	Ogre::Real outerRadius = mBrush->getOuterRadius();
	int size = (int)(2*outerRadius) + 2;
	int left = (int)position.x;
	int top = (int)position.z;
	Ogre::Rect editRect(
		std::max(0,left-size/2), 
		std::max(0,top-size/2), 
		std::min(heightfieldBuffer->getElementColumnCount(), left + size/2),
		std::min(heightfieldBuffer->getElementRowCount(), top + size/2));
	Ogre::Real* data = new Ogre::Real[editRect.width() * editRect.height()];

	Ogre::PixelBox pixelBox(Utils::rectToBox(editRect),Ogre::PF_FLOAT32_R, data);
	heightfieldBuffer->copyTo(pixelBox);
	Ogre::Real scale = 1.0f / (innerRadius*innerRadius-outerRadius*outerRadius);
	Ogre::Real bias = -outerRadius * outerRadius * scale;

	Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() - heightfieldBuffer->getHeightRangeMin();
	Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
	Ogre::Real cy = position.y;
	Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

	for (int x = 0; x < (int)editRect.width(); ++x)
	{
		for (int z = 0; z < (int)editRect.height(); ++z)
		{
/*			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dy = (Ogre::Real)y - cy;

			Ogre::Real s = dx * dx + dy * dy;
			Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
			data[y*editRect.width()+x] = Utils::clamp(data[y*editRect.width()+x] + delta, 0.0f, 1.0f);
*/
			Ogre::Real hy = data[z*editRect.width()+x];
			Ogre::Real dx = (Ogre::Real)x - cx;
			Ogre::Real dy = hy * heightscale - cy;
			Ogre::Real dz = (Ogre::Real)z - cz;

			Ogre::Real s = dx * dx + dz * dz;
			Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f); // standard lower
			//delta = delta * Utils::clamp(1.0f + dy / innerRadius, 0.0f, 1.0f) ; // tunneler
			delta = delta * Utils::clamp(dy * dy * scale + bias, 0.0f, 1.0f); //steepener/leveller (better?)
			//delta = delta * dy; // leveller
			data[z*editRect.width()+x] = Utils::clamp(hy + delta / heightscale, 0.0f, 1.0f);

		}
	}
	heightfieldBuffer->updateFrom(pixelBox);
	heightfieldBuffer->revalidate();
	delete data;
}
#endif
