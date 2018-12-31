/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "ShaderCustomAutoConstants.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
std::vector<Utils::GUID> ShaderCustomAutoConstants::mGlobalActiveAutoConstantGUIDs;

// ----------------------------------------------------------------------------
ShaderCustomAutoConstants::~ShaderCustomAutoConstants() { clear(); }

// ----------------------------------------------------------------------------
void ShaderCustomAutoConstants::setConstant(size_t constantIndex, const Ogre::Real* data, int dataCount)
{
    setConstant(constantIndex, data, dataCount, false);
}

// ----------------------------------------------------------------------------
void ShaderCustomAutoConstants::setConstant(size_t constantIndex, const Ogre::Vector4& data)
{
    setConstant(constantIndex, data.ptr(), 4, false);
}

// ----------------------------------------------------------------------------
void ShaderCustomAutoConstants::setConstant(size_t constantIndex, const Ogre::Matrix4& data)
{
    setConstant(constantIndex, data[0], 16, true);
}

// ----------------------------------------------------------------------------
void ShaderCustomAutoConstants::clear()
{
    AutoConstant resetConstant;
    for (size_t constantIndex = 0; constantIndex < mAutoConstants.size(); ++constantIndex)
    {
        AutoConstant& autoConstant = mAutoConstants[constantIndex];
        if (autoConstant.data)
        {
            delete[] autoConstant.data;
        }
        autoConstant = resetConstant;
    }

    mAutoConstants.clear();
}

// ----------------------------------------------------------------------------
bool ShaderCustomAutoConstants::updateShaderParameter(
    Ogre::GpuProgramParameters* params,
    const Ogre::GpuProgramParameters::AutoConstantEntry& autoConstantEntry) const
{
    if (autoConstantEntry.data < mAutoConstants.size())
    {
        const AutoConstant& constant = mAutoConstants[autoConstantEntry.data];
        if (constant.guid && constant.guid != mGlobalActiveAutoConstantGUIDs[autoConstantEntry.data])
        {
            if (constant.isMatrix && constant.dataCount == 16 && params->getTransposeMatrices())
            {
                Ogre::Matrix4* m = (Ogre::Matrix4*)constant.data;
                Ogre::Matrix4 t = m->transpose();
                params->_writeRawConstant(autoConstantEntry.physicalIndex, &t, constant.dataCount);
            }
            else
            {
                params->_writeRawConstants(autoConstantEntry.physicalIndex, constant.data,
                                           constant.dataCount);
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------------
void ShaderCustomAutoConstants::setConstant(size_t constantIndex, const Ogre::Real* data, int dataCount,
                                            bool isMatrix)
{
    if (constantIndex >= mAutoConstants.size())
    {
        mAutoConstants.resize(constantIndex + 1, AutoConstant());
        if (mAutoConstants.size() >= mGlobalActiveAutoConstantGUIDs.size())
        {
            mGlobalActiveAutoConstantGUIDs.resize(mAutoConstants.size() + 10, 0);
        }
    }
    AutoConstant& autoConstant = mAutoConstants[constantIndex];
    autoConstant.guid = Utils::createGUID();
    autoConstant.dataCount = dataCount;
    autoConstant.isMatrix = false;

    if (!autoConstant.data || autoConstant.dataCount > autoConstant.dataCount)
    {
        delete[] autoConstant.data;
        autoConstant.data = new Ogre::Real[autoConstant.dataCount];
    }

    memcpy((void*)autoConstant.data, (void*)data, dataCount * sizeof(Ogre::Real));
}
