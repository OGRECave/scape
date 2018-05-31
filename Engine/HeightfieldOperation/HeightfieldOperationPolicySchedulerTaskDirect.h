/**
 * ScapeEngine::HeightfieldOperationBrushInstancr class
 *
 * Base class for processing brush instances one by one without splitting them in pages
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONPOLICYSCHEDULERDIRECTTASK_H__
#define __HEIGHTFIELDOPERATIONPOLICYSCHEDULERDIRECTTASK_H__

#include "HeightfieldOperationTaskDirect.h"
#include "HeightfieldOperationPolicyScheduler.h"
#include "HeightfieldOperationPolicyBrushInstancerSpacer.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"

namespace ScapeEngine
{
	static const int MINWORKMILLIS = 100;
	class HeightfieldOperationTaskDirect;

	
	template<class SubClass, class ParentClass>
	class HeightfieldOperationPolicySchedulerTaskDirect : public ParentClass
	{
	protected:
		HeightfieldOperationPolicySchedulerTaskDirect() : ParentClass() {}


		// ----------------------------------------------------------------------------
		virtual void schedulerTick()
		{
			SubClass* operation = static_cast<SubClass*>(this);

			size_t allowedWorkingBrushInstances = 1000;

			if (mHeightfieldOperationTaskDirectList.empty())
			{
				mWorkingBrushInstancesCount = 0;
			}

			while (operation->getBrushInstanceCount() &&  mWorkingBrushInstancesCount < allowedWorkingBrushInstances)
			{
				BrushInstance brushInstance = operation->popBrushInstance();
				HeightfieldOperationTaskDirect* operationDirect = operation->createTaskDirect(brushInstance);
				mHeightfieldOperationTaskDirectList.push_back(operationDirect);
			}

			bool isDone = false;
			while (!isDone)
			{
				isDone = true;
				// Process current list of HeightfieldOperationDirects
				HeightfieldOperationTaskDirectList::iterator directIt, directItEnd = mHeightfieldOperationTaskDirectList.end();
				for (directIt = mHeightfieldOperationTaskDirectList.begin(); directIt != directItEnd;)
				{
					HeightfieldOperationTaskDirect& direct = **directIt;

					bool tryMore = true;
					while (tryMore)
					{
						direct.tick();
						tryMore = direct.isActive();

						if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - this->mLastTickMillis >= MINWORKMILLIS)
						{
							break;
						}
					}

					if (direct.isCompleted())
					{
						operation->getInOutHeightfieldBuffer()->updateGeoms(direct.getUpdatedRect());

						delete *directIt;
						mHeightfieldOperationTaskDirectList.erase(directIt++);	
					}
					else
					{
						++directIt;
						isDone = false;
					}

					if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - this->mLastTickMillis >= MINWORKMILLIS)
					{
						break;
					}
				}
			}

			//mLastTickMillis = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

		}


		// ----------------------------------------------------------------------------
		virtual bool isSchedulerDone()
		{
			return mHeightfieldOperationTaskDirectList.empty();
		}

	private:
		HeightfieldOperationTaskDirectList mHeightfieldOperationTaskDirectList;
		size_t mWorkingBrushInstancesCount;
	};
}

#endif // __HEIGHTFIELDOPERATIONPOLICYSCHEDULERDIRECTTASK_H__
