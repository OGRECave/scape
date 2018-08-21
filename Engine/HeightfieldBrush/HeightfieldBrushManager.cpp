/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldBrush.h"
#include "HeightfieldBrushManager.h"
#include "HeightfieldOperation/HeightfieldOperation.h"
#include "HeightfieldOperation/HeightfieldOperationStack.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
HeightfieldBrushManager::HeightfieldBrushManager()
{
}


// ----------------------------------------------------------------------------
HeightfieldBrushManager::~HeightfieldBrushManager()
{
	Brushes::iterator brushIt = mBrushes.begin(), brushItEnd = mBrushes.end();
	for (; brushIt != brushItEnd; ++brushIt)
	{
		delete brushIt->second;
	}
}


// ----------------------------------------------------------------------------
HeightfieldBrush* HeightfieldBrushManager::getCurrentBrush() const
{
	HeightfieldOperation* operation = getEngineCore()->getHeightfieldOperationStack()->getLastOperation();
	if (!operation || !operation->isActive())
	{
		operation = getEngineCore()->getHeightfieldOperationStack()->getNewOperation();
	}

	if (operation)
	{
		return operation->getBrush();
	}
	return NULL;
}


// ----------------------------------------------------------------------------
HeightfieldBrush* HeightfieldBrushManager::getBrush(const string& brushName)
{
	HeightfieldBrush* brush = mBrushes[brushName];
	if (brush == NULL)
	{
		brush = mBrushes[brushName] = new HeightfieldBrush();
		brush->setInnerRadius(50.f);
		brush->setOuterRadius(250.f);
	}
	//printf("%s %p\n", brushName.c_str(), brush);
	return brush;
}
