/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldOperationFactory.h"
#include "HeightfieldOperation/HeightfieldOperationCPUBrush.h"
#include "HeightfieldOperation/HeightfieldOperationCPUBrush2.h"
#include "HeightfieldOperation/HeightfieldOperationCPUErosion.h"
#include "HeightfieldOperation/HeightfieldOperationCPUNoise.h"
#include "HeightfieldOperation/HeightfieldOperationCPUSmooth.h"
#include "HeightfieldOperation/HeightfieldOperationCPULevel.h"
#include "HeightfieldOperation/HeightfieldOperationGPUBrush.h"
#include "HeightfieldOperation/HeightfieldOperationGPUPencil.h"
#include "HeightfieldOperation/HeightfieldOperationGPUNoise.h"
#include "HeightfieldOperation/HeightfieldOperationGPUNoiseDrag.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
void HeightfieldOperationFactory::registerClass(const string& className, HeightfieldOperation*(*classCreateMethod)())
{
	assert(mOperationDescMap.find(className) == mOperationDescMap.end() && _T("HeightfieldOperation class already registered"));
	mOperationDescMap[className] = classCreateMethod;
}


// ----------------------------------------------------------------------------
void HeightfieldOperationFactory::registerClasses()
{
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPUBrush);
//	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPUBrush2);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPUErosion);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPUNoise);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPUSmooth);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationCPULevel);

	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationGPUBrush);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationGPUPencil);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationGPUNoise);
	REGISTER_HEIGHTFIELDOPERATION_CLASS(this, HeightfieldOperationGPUNoiseDrag);
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationFactory::isRegistered(const string& className)
{
	OperationDescMap::iterator operationDescMapIt = mOperationDescMap.find(className);
	return operationDescMapIt != mOperationDescMap.end();
}


// ----------------------------------------------------------------------------
StringList HeightfieldOperationFactory::getOperationNameList()
{
	StringList descs;

	OperationDescMap::iterator operationDescMapIt, operationDescMapItEnd = mOperationDescMap.end();
	for (operationDescMapIt = mOperationDescMap.begin(); operationDescMapIt != operationDescMapItEnd; ++operationDescMapIt)
	{
		descs.push_front(operationDescMapIt->first);
	}
	return descs;
}


// ----------------------------------------------------------------------------
HeightfieldOperation* HeightfieldOperationFactory::create(const string& className)
{
	OperationDescMap::iterator operationDescMapIt = mOperationDescMap.find(className);
	if (operationDescMapIt == mOperationDescMap.end())
	{
		OGRE_EXCEPT(
			Ogre::Exception::ERR_ITEM_NOT_FOUND, 
			_T("Tried to create unknown class type '") + className + _T("'for HeightfieldOperationFactory"),
			_T("HeightfieldOperationFactory::create"));
	}
	return operationDescMapIt->second();
}


