/**
 * ScapeEngine::HeightfieldOperationBrushInstancr class
 *
 * Base class for splitting multiple brush instances into paged tasks, and process per page
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONPOLICYSCHEDULERTASKPAGEBATCH_H__
#define __HEIGHTFIELDOPERATIONPOLICYSCHEDULERTASKPAGEBATCH_H__

#include "HeightfieldOperationPolicyScheduler.h"
#include "HeightfieldOperationTaskPage.h"

namespace ScapeEngine
{
class HeightfieldBrush;
class HeightfieldOperationTaskPage;

template <class SubClass, class ParentClass>
class HeightfieldOperationPolicySchedulerTaskPageBatch : public ParentClass
{
protected:
    HeightfieldOperationPolicySchedulerTaskPageBatch()
        : ParentClass(), mWorkingBrushInstancesCount(0), mAllowMultipleBrushInstances(false)
    {
    }

    // ----------------------------------------------------------------------------
    virtual void allowMultipleBrushInstances(bool allow) { mAllowMultipleBrushInstances = allow; }

    // ----------------------------------------------------------------------------
    virtual void schedulerTick()
    {
        SubClass* operation = static_cast<SubClass*>(this);

        if (mPageBinnedBrushInstances.empty() && mHeightfieldOperationTaskPageList.empty())
        {
            mWorkingBrushInstancesCount = 0;
        }

        size_t allowedWorkingBrushInstances = mAllowMultipleBrushInstances ? 1000 : 1;

        while (this->getBrushInstanceCount() && mWorkingBrushInstancesCount < allowedWorkingBrushInstances)
        {
            BrushInstance brushInstance = this->popBrushInstance();
            HeightfieldBuffer* inOutHeightfieldBuffer = this->getInOutHeightfieldBuffer();

            int radius = (int)(brushInstance.outerRadius) + 2;
            int column = (int)brushInstance.position.x;
            int row = (int)brushInstance.position.z;
            Ogre::Rect editRect(
                std::max(0, column - radius), std::max(0, row - radius),
                std::min(inOutHeightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(),
                         column + radius),
                std::min(inOutHeightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(),
                         row + radius));

            if (editRect.width() > 0 && editRect.height() > 0)
            {
                Ogre::Rect pageRect(inOutHeightfieldBuffer->getPageColumnFromElement(editRect.left),
                                    inOutHeightfieldBuffer->getPageRowFromElement(editRect.top),
                                    inOutHeightfieldBuffer->getPageColumnFromElement(editRect.right - 1) +
                                        1,
                                    inOutHeightfieldBuffer->getPageRowFromElement(editRect.bottom - 1) + 1);

                for (int pageColumn = pageRect.left; pageColumn < pageRect.right; ++pageColumn)
                {
                    for (int pageRow = pageRect.top; pageRow < pageRect.bottom; ++pageRow)
                    {
                        mPageBinnedBrushInstances[BrushPageCoords(pageColumn, pageRow)].push_back(
                            brushInstance);
                    }
                }
            }

            ++mWorkingBrushInstancesCount;
        }

        BrushInstancesGrid::iterator gridIt = mPageBinnedBrushInstances.begin();
        BrushInstancesGrid::iterator gridItEnd = mPageBinnedBrushInstances.end();

        while (gridIt != gridItEnd)
        {
            const BrushInstances& brushInstances = gridIt->second;
            BrushPageCoords pageCoords(gridIt->first.first, gridIt->first.second);

            bool queuedInstances = false;
            bool foundPage = false;

            HeightfieldOperationTaskPageList::iterator pageIt,
                pageItEnd = mHeightfieldOperationTaskPageList.end();
            for (pageIt = mHeightfieldOperationTaskPageList.begin(); pageIt != pageItEnd; ++pageIt)
            {
                if ((*pageIt)->getPageCoords() == pageCoords)
                {
                    foundPage = true;
                    if ((*pageIt)->addBrushInstances(brushInstances))
                    {
                        queuedInstances = true;
                        // brush instances are merged with page in list
                        break;
                    }
                }
            }

            if (!foundPage)
            {
                // new page found, so add new one for the current brushinstanes
                HeightfieldOperationTaskPage* operationPage =
                    operation->createTaskPage(pageCoords, brushInstances);
                mHeightfieldOperationTaskPageList.push_back(operationPage);
                queuedInstances = true;
            }

            if (queuedInstances)
            {
                mPageBinnedBrushInstances.erase(gridIt++);
            }
            else
            {
                ++gridIt;
            }
        }

        // parallel page processing, without updating inout buffer. only used for disallowed
        // multiplebrushinstances
        bool isDone = false;
        while (!isDone && !mAllowMultipleBrushInstances)
        {
            isDone = true;
            // Process current list of HeightfieldOperationPages
            HeightfieldOperationTaskPageList::iterator pageIt,
                pageItEnd = mHeightfieldOperationTaskPageList.end();
            for (pageIt = mHeightfieldOperationTaskPageList.begin(); pageIt != pageItEnd;)
            {
                HeightfieldOperationTaskPage& page = **pageIt;
                page.tick();

                if (page.isCompleted())
                {
                    ++pageIt;
                }
                else
                {
                    ++pageIt;
                    isDone = false;
                }

                if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - this->mLastTickMillis >=
                    MINWORKMILLIS)
                {
                    break;
                }
            }
        }

        // parallel page processing, without updating inout buffer from temp and updating geoms. when
        // disallowed multiplebrushinstances,
        // only executed when all pages are already done
        if (mAllowMultipleBrushInstances || (!mAllowMultipleBrushInstances && isDone))
        {
            // Process current list of HeightfieldOperationPages
            HeightfieldOperationTaskPageList::iterator pageIt,
                pageItEnd = mHeightfieldOperationTaskPageList.end();
            for (pageIt = mHeightfieldOperationTaskPageList.begin(); pageIt != pageItEnd;)
            {
                HeightfieldOperationTaskPage& page = **pageIt;
                page.tick();

                if (page.isCompleted())
                {
                    this->getInOutHeightfieldBuffer()->swapPage(page.getPageCoords().first,
                                                                page.getPageCoords().second,
                                                                this->getTempHeightfieldBuffer());
                    this->getInOutHeightfieldBuffer()->updateGeoms(page.getUpdatedRect());

                    delete (*pageIt);
                    mHeightfieldOperationTaskPageList.erase(pageIt++);
                }
                else
                {
                    ++pageIt;
                    isDone = false;
                }

                if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - this->mLastTickMillis >=
                    MINWORKMILLIS)
                {
                    break;
                }
            }
        }
    }

    // ----------------------------------------------------------------------------
    virtual bool isSchedulerDone()
    {
        return mHeightfieldOperationTaskPageList.empty() && mPageBinnedBrushInstances.empty();
    }

private:
    BrushInstancesGrid mPageBinnedBrushInstances;
    HeightfieldOperationTaskPageList mHeightfieldOperationTaskPageList;
    size_t mWorkingBrushInstancesCount;
    size_t mAllowMultipleBrushInstances;
};
}

#endif // __HEIGHTFIELDOPERATIONPOLICYSCHEDULERTASKPAGEBATCH_H__
