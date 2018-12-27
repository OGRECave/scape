/**
 * ScapeEngine::HeightfieldOperationPolicyBrushInstancerSpacer class
 *
 * A HeightfieldOperationBrushInstancr implementation that tries to space instances evenly across a spline
 * generated from brush positions
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCERSPACER_H__
#define __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCERSPACER_H__

#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldOperationPolicyBrushInstancer.h"

namespace ScapeEngine
{
class HeightfieldBrush;

template <class SubClass, class ParentClass>
class HeightfieldOperationPolicyBrushInstancerSpacer : public ParentClass
{
protected:
    // ----------------------------------------------------------------------------
    HeightfieldOperationPolicyBrushInstancerSpacer()
        : mIsActiveStroke(true), mWasActiveStroke(false), mHasLastBrushInstancePosition(false),
          mPathNextParam(0.0f)
    {
    }

    // ----------------------------------------------------------------------------
    virtual void brushInstancerTick()
    {
        HeightfieldBrush* brush = this->getBrush();
        assert(brush);

        bool brushActive = brush->isActive() && brush->getHeightfieldGeom() && mIsActiveStroke;

        if (brush->getHeightfieldGeom())
        {
            mCurrentPathNode.position = brush->getHeightfieldGeom()->getLocalPosition(brush->getPosition());
        }
        mCurrentPathNode.pressure = brush->getPressure();
        mCurrentPathNode.time = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
        mCurrentPathNode.primary = brush->isPrimaryActive();

        if (brushActive != mWasActiveStroke)
        {
            BrushPathNode transitionNode = mCurrentPathNode;
            transitionNode.pressure = 0.0f;
            transitionNode.time -= 2;
            this->mBrushPathNodes.push_back(transitionNode);
            transitionNode.time += 1;
            this->mBrushPathNodes.push_back(transitionNode);
        }

        if (brushActive)
        {
            this->mBrushPathNodes.push_back(mCurrentPathNode);
            // brushActive = false;
        }
        mWasActiveStroke = mIsActiveStroke;
        mIsActiveStroke = brush->isActive() && mIsActiveStroke;

        Ogre::Real innerRadius = brush->getInnerRadius();
        Ogre::Real outerRadius = brush->getOuterRadius();
        Ogre::Real rampPower = brush->getRampPower();

        if (this->isPencilMode())
        {
            while (this->mBrushPathNodes.size() >= 4)
            {
                BrushPathNodes::iterator nodesIt = this->mBrushPathNodes.begin();

                BrushPathNode& node1 = *nodesIt++;
                BrushPathNode& node2 = *nodesIt++;
                BrushPathNode& node3 = *nodesIt++;
                BrushPathNode& node4 = *nodesIt++;

                Ogre::Matrix4 splineVectors(node1.position.x, node2.position.x, node3.position.x,
                                            node4.position.x, node1.position.y, node2.position.y,
                                            node3.position.y, node4.position.y, node1.position.z,
                                            node2.position.z, node3.position.z, node4.position.z,
                                            node1.pressure, node2.pressure, node3.pressure, node4.pressure);

                Ogre::Vector3 subPoint1 = node2.position;
                Ogre::Vector3 subPoint2 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.25f)).ptr());
                Ogre::Vector3 subPoint3 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.5f)).ptr());
                Ogre::Vector3 subPoint4 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.75f)).ptr());
                Ogre::Vector3 subPoint5 = node3.position;

                Ogre::Real distance =
                    std::max(1.0f, subPoint1.distance(subPoint2) + subPoint2.distance(subPoint3) +
                                       subPoint3.distance(subPoint4) + subPoint4.distance(subPoint5));

                distance = std::max(0.1f, node2.position.distance(node3.position));
                Ogre::Real stepSize = std::max(
                    1.0f, this->mPathSpacing * (brush->getOuterRadius() - brush->getInnerRadius()));
                Ogre::Real stepStrength =
                    stepSize / brush->getOuterRadius() * std::max(0.1f, 0.001f * (node3.time - node2.time));
                // Ogre::Real stepStrength = std::max(0.1f, 0.001f * (node3.time - node2.time));

                Ogre::Vector3 position;
                Ogre::Real strength;
                if (mPathNextParam < distance)
                {
                    Ogre::Vector4 weights = Utils::getCatmullRomeSplineWeights(mPathNextParam / distance);
                    Ogre::Vector4 interpolation = splineVectors * weights;
                    position = Ogre::Vector3(interpolation.ptr());
                    strength = 0.5f * stepStrength * interpolation.w * interpolation.w *
                               this->getStrength() *
                               sqrtf(Utils::lerp(rampPower, 1.0f, innerRadius / outerRadius));

                    if (!mHasLastBrushInstancePosition)
                    {
                        mLastBrushInstancePosition = position;
                        mHasLastBrushInstancePosition = true;
                    }

                    if (strength > 0.0f)
                    {
                        strength = (node2.primary ? 1.0f : -1.0f) * strength;
                        BrushInstance instance;
                        instance.position = position;
                        instance.direction =
                            (instance.position - mLastBrushInstancePosition).normalisedCopy();
                        instance.strength = strength;
                        instance.innerRadius = innerRadius;
                        instance.outerRadius = outerRadius;
                        instance.rampPower = rampPower;
                        this->mBrushInstances.push_back(instance);
                    }

                    mPathNextParam += stepSize;
                    mLastBrushInstancePosition = position;
                }
                else
                {
                    mPathNextParam -= distance;
                    this->mBrushPathNodes.pop_front();
                }
            }
        }
        else
        {
            while (this->mBrushPathNodes.size() >= 1)
            {
                BrushPathNodes::iterator nodesIt = this->mBrushPathNodes.begin();

                if (!mHasLastBrushInstancePosition)
                {
                    mLastBrushInstancePosition = nodesIt->position;
                    mHasLastBrushInstancePosition = true;
                }

                if (nodesIt->pressure > 0.0f)
                {
                    Ogre::Real strength = 0.1f * nodesIt->pressure * this->getStrength();
                    strength = (nodesIt->primary ? 1.0f : -1.0f) * strength;
                    BrushInstance instance;
                    instance.position = nodesIt->position;
                    instance.direction = (instance.position - mLastBrushInstancePosition).normalisedCopy();
                    instance.strength = strength;
                    instance.innerRadius = innerRadius;
                    instance.outerRadius = outerRadius;
                    instance.rampPower = rampPower;
                    this->mBrushInstances.push_back(instance);
                }
                mLastBrushInstancePosition = nodesIt->position;
                this->mBrushPathNodes.pop_front();
            }
        }
    }

private:
    BrushPathNode mCurrentPathNode;
    Ogre::Real mPathNextParam;

    bool mIsActiveStroke;
    bool mWasActiveStroke;

    Ogre::Vector3 mLastBrushInstancePosition;
    bool mHasLastBrushInstancePosition;

    // Ogre::Real mPathSpacing;
};
}

#endif // __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCERSPACER_H__
