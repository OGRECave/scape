/**
 * ScapeEngine::HeightfieldOperation class
 *
 * An abstract super class for an HeightfieldPage operation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATION_H__
#define __HEIGHTFIELDOPERATION_H__

#include "EngineCore/UIElementClasses.h"

namespace ScapeEngine
{
	class HeightfieldBrush;
	class HeightfieldBuffer;
	class HeightfieldOperationTaskPage;

	class HeightfieldOperation : public UIElementPresetContainerSimple, public FactoryClass<HeightfieldOperation>
	{
	public:
		enum EState
		{
			STATE_DO,
			STATE_REDO,
			STATE_UNDO
		};

		virtual ~HeightfieldOperation() {}
		virtual bool canInitiate(EState state) = 0;
		virtual bool isActive() = 0;
		virtual bool isActive(EState state) = 0;
		virtual bool isDone(EState state) = 0;
		virtual void initiate(EState state, HeightfieldBuffer* inOutHeightfieldBuffer) = 0;
		virtual void tick() = 0;
		
		virtual HeightfieldBuffer* getInOutHeightfieldBuffer() = 0;
		virtual HeightfieldBuffer* getTempHeightfieldBuffer() = 0;

		virtual HeightfieldBrush* getBrush() {return NULL;}

	protected:
		HeightfieldOperation() {}
	};
}

#endif // __HEIGHTFIELDOPERATION_H__
