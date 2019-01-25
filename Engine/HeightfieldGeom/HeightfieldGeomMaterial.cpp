/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldGeomMaterial.h"
#include "HeightfieldGeom.h"
#include "HeightfieldGeomTile.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "Utils/ShaderCustomAutoConstants.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "HeightfieldBrush/HeightfieldBrushSettings.h"
#include "HeightfieldBrush/HeightfieldBrushState.h"

using namespace ScapeEngine;

HeightfieldGeomMaterial::HeightfieldGeomMaterial()
    : mInitialized(false), mBrush(NULL),
      mHeightfieldBrushSettings(
          getEngineCore()->getHeightfieldBrushManager()->getHeightfieldBrushSettings()),
      mHeightfieldBrushState(getEngineCore()->getHeightfieldBrushManager()->getHeightfieldBrushState()),
      mHasDirtySettings(true)
{
    mMaterial.reset();
}

HeightfieldGeomMaterial::~HeightfieldGeomMaterial() {}

void HeightfieldGeomMaterial::initialize()
{
    if (!mInitialized)
    {
        bindShaderConstants();
        mInitialized = true;
    }
}

const Ogre::MaterialPtr& HeightfieldGeomMaterial::getMaterial()
{
    if (!mMaterial)
    {
        mMaterial = createMaterial();
        assert(mMaterial);
    }

    return mMaterial;
}

void HeightfieldGeomMaterial::updateShaderConstantsGeom(
    const HeightfieldGeom* heightfieldGeom, ShaderCustomAutoConstants* shaderCustomAutoConstants,
    bool forceAll)
{
    assert(shaderCustomAutoConstants);

    initialize();

    assert(heightfieldGeom);

    Ogre::Real worldQuadSize = heightfieldGeom->getWorldQuadSize();

    if (forceAll)
    {
        const HeightfieldBuffer* heightfieldBuffer = heightfieldGeom->getHeightfieldBuffer();
        assert(heightfieldBuffer);

        const Ogre::Real rawRangeMinHeight =
            Utils::getTypicalVertexElementRangeMin(heightfieldGeom->getVertexElementTypeHeight());
        const Ogre::Real rawRangeMaxHeight =
            Utils::getTypicalVertexElementRangeMax(heightfieldGeom->getVertexElementTypeHeight());
        const Ogre::Real rawRangeScale = 1.0f / (rawRangeMaxHeight - rawRangeMinHeight);
        const Ogre::Real unitQuadRangeMinHeight =
            heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMin();
        const Ogre::Real unitQuadRangeMaxHeight =
            heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMax();
        const Ogre::Real unitQuadScaleHeight =
            (unitQuadRangeMaxHeight - unitQuadRangeMinHeight) * rawRangeScale;

        // quad space to object space scaling factor
        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::LOCALPOSSCALE,
            Ogre::Vector4(worldQuadSize, worldQuadSize, worldQuadSize, 1.0f));

        // height difference to gradient scaling and bias factors
        shaderCustomAutoConstants->setConstant(_HeightfieldGeomMaterialNS::GRADIENTSCALEBIAS,
                                               Ogre::Vector4(unitQuadScaleHeight / worldQuadSize,
                                                             0.0f,   // not used
                                                             0.0f,   // not used
                                                             0.0f)); // not used
    }

    // brush settings
    if (mBrush)
    {
        Ogre::Vector3 worldBrushPosition = mHeightfieldBrushState.getPosition();
        Ogre::Real brushInnerRadius = mHeightfieldBrushSettings.getInnerRadius() / worldQuadSize;
        Ogre::Real brushOuterRadius =
            std::max(brushInnerRadius * 1.01f, mHeightfieldBrushSettings.getOuterRadius() / worldQuadSize);
        Ogre::Vector3 brushPosition = heightfieldGeom->getLocalPosition(worldBrushPosition);
        Ogre::Real inner = powf(brushInnerRadius, 2.0f); // mBrush->getRampPower() *
                                                         // mBrush->getRampPower());
        Ogre::Real outer = powf(brushOuterRadius, 2.0f); // mBrush->getRampPower() *
                                                         // mBrush->getRampPower());
        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::BRUSHPOSRAMP,
            Ogre::Vector4(brushPosition.x,                  // U coordinate of brush center
                          brushPosition.z,                  // V coordinate of brush center
                          -1.0f / (outer - inner),          // brush ramp scale
                          1.0f + inner / (outer - inner))); // brush ramp bias

        // std::cout << mBrush->getRampPower() << "  " << inner << "   " << outer << "\n";

        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::BRUSHCOLOR,
            Ogre::Vector4(1.0f,                                              // r
                          0.0f,                                              // g
                          0.0f,                                              // b
                          1.0f * mHeightfieldBrushSettings.getRampPower())); // brush shape power
    }
    else
    {
        shaderCustomAutoConstants->setConstant(_HeightfieldGeomMaterialNS::BRUSHPOSRAMP,
                                               Ogre::Vector4(0,   // U coordinate of brush center
                                                             0,   // V coordinate of brush center
                                                             0,   // brush ramp scale
                                                             0)); // brush ramp bias

        shaderCustomAutoConstants->setConstant(_HeightfieldGeomMaterialNS::BRUSHCOLOR,
                                               Ogre::Vector4(1.0f,   // r
                                                             0.0f,   // g
                                                             0.0f,   // b
                                                             0.5f)); // brush shape power
    }
}

void HeightfieldGeomMaterial::updateShaderConstantsGeomTile(
    const HeightfieldGeomTile* heightfieldGeomTile, ShaderCustomAutoConstants* shaderCustomAutoConstants,
    bool forceAll)
{
    assert(heightfieldGeomTile);
    assert(shaderCustomAutoConstants);

    initialize();

    const HeightfieldGeom* heightfieldGeom = heightfieldGeomTile->getHeightfieldGeom();
    assert(heightfieldGeom);

    Ogre::Real worldQuadSize = heightfieldGeom->getWorldQuadSize();

    if (forceAll)
    {
        const HeightfieldBuffer* heightfieldBuffer = heightfieldGeom->getHeightfieldBuffer();
        assert(heightfieldBuffer);

        const Ogre::Real rawRangeMinHeight =
            Utils::getTypicalVertexElementRangeMin(heightfieldGeom->getVertexElementTypeHeight());
        const Ogre::Real rawRangeMaxHeight =
            Utils::getTypicalVertexElementRangeMax(heightfieldGeom->getVertexElementTypeHeight());
        const Ogre::Real rawRangeMinUV =
            Utils::getTypicalVertexElementRangeMin(heightfieldGeom->getVertexElementTypeUV());
        const Ogre::Real rawRangeMaxUV =
            Utils::getTypicalVertexElementRangeMax(heightfieldGeom->getVertexElementTypeUV());
        const Ogre::Real rawRangeScale = 1.0f / (rawRangeMaxHeight - rawRangeMinHeight);
        const Ogre::Real rawRangeBias = -rawRangeMinHeight * rawRangeScale;
        const Ogre::Real unitQuadRangeMinHeight =
            heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMin();
        const Ogre::Real unitQuadRangeMaxHeight =
            heightfieldBuffer->getHeightfieldBufferSet()->getHeightRangeMax();
        const Ogre::Real unitQuadBiasHeight =
            (unitQuadRangeMaxHeight - unitQuadRangeMinHeight) * rawRangeBias -
            0.5f * unitQuadRangeMinHeight;
        const Ogre::Real unitQuadScaleU =
            1.0f / (rawRangeMaxUV - rawRangeMinUV) * heightfieldGeomTile->getTemplateQuadColumnCount();
        const Ogre::Real unitQuadBiasU =
            -rawRangeMinUV * unitQuadScaleU + (Ogre::Real)heightfieldGeomTile->getVisibleQuadRect().left;
        const Ogre::Real unitQuadScaleV =
            1.0f / (rawRangeMaxUV - rawRangeMinUV) * heightfieldGeomTile->getTemplateQuadRowCount();
        const Ogre::Real unitQuadBiasV =
            -rawRangeMinUV * unitQuadScaleV + (Ogre::Real)heightfieldGeomTile->getVisibleQuadRect().top;
        const Ogre::Real unitQuadScaleHeight =
            (unitQuadRangeMaxHeight - unitQuadRangeMinHeight) * rawRangeScale;

        // UYV space to quad space scaling factor (1 quad rectangle = 1 unit, both vertically and
        // horizontally)
        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::UNITQUADSCALE,
            Ogre::Vector4(unitQuadScaleU, //* 0.995f, //TODO GdCarpentier 10/18/2007 :
                          unitQuadScaleHeight,
                          unitQuadScaleV, //* 0.995f, //TODO GdCarpentier 10/18/2007 :
                          1.0f));

        // UYV space to quad space bias factor (1 quad rectangle = 1 unit, both vertically and horizontally)
        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::UNITQUADBIAS,
            Ogre::Vector4(unitQuadBiasU, unitQuadBiasHeight, unitQuadBiasV, 0.0f));

        // quad space to object space bias factor
        shaderCustomAutoConstants->setConstant(
            _HeightfieldGeomMaterialNS::LOCALPOSBIAS,
            Ogre::Vector4(heightfieldGeomTile->getBoundingBox().getMinimum().x -
                              (Ogre::Real)heightfieldGeomTile->getVisibleQuadRect().left * worldQuadSize,
                          heightfieldGeomTile->getBoundingBox().getMinimum().y,
                          heightfieldGeomTile->getBoundingBox().getMinimum().z -
                              (Ogre::Real)heightfieldGeomTile->getVisibleQuadRect().top * worldQuadSize,
                          0.0f));
    }
}

void HeightfieldGeomMaterial::bindShaderConstants()
{
    Ogre::MaterialPtr material = getMaterial();
    assert(material);

    Ogre::Technique* technique = material->getBestTechnique();
    assert(technique);

    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::UNITQUADBIAS,
                                        _T("unitQuadBias"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::UNITQUADSCALE,
                                        _T("unitQuadScale"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::LOCALPOSSCALE,
                                        _T("localPosScale"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::LOCALPOSBIAS,
                                        _T("localPosBias"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::GRADIENTSCALEBIAS,
                                        _T("gradientScaleBias"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::BRUSHPOSRAMP,
                                        _T("brushPosRamp"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialNS::BRUSHCOLOR,
                                        _T("brushColor"));
}

void HeightfieldGeomMaterial::bindBrush(class HeightfieldBrush* brush) { mBrush = brush; }
