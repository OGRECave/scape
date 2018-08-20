/**
 * ScapeEngine::GPU2DOperation class
 *
 * Render-to-texture operation, using textured GPU2DOperationQuads as its input
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATION_H__
#define __GPU2DOPERATION_H__

#include "GPU2DOperationQuad.h"

namespace ScapeEngine {
    class GPU2DOperation
    {
    public:
        GPU2DOperation();
        ~GPU2DOperation();

        void clearQuads();

        void addQuad(GPU2DOperationQuadPtr quad);

        void bindMultiRenderTarget(Ogre::TexturePtr renderTargetTexture);

        void bindSingleRenderTarget(Ogre::RenderTarget* renderTarget);

        Ogre::TexturePtr getRenderTargetTexture() { return *mRenderTargetTextures.begin(); }
        void setCameraPosition(int x, int y);

        void render(bool clearFirst);

        void blitToPixelBox(const Ogre::PixelBox& pixelBox);

        void clearDirtyRegion() { mDirtyRegion = Ogre::Rect(0, 0, 0, 0); }
        void addDirtyRegion(const Ogre::Rect& rect) { mDirtyRegion = Utils::unionTRect(mDirtyRegion, rect); }
        const Ogre::Rect& getDirtyRegion() { return mDirtyRegion; }
    protected:
        Ogre::Camera* mCamera;
        Ogre::Viewport* mViewport;
        Ogre::SceneManager* mSceneManager;
        Ogre::Rect mDirtyRegion;
        std::list<GPU2DOperationQuadPtr> mGPU2DOperationQuads;
        std::list<Ogre::TexturePtr> mRenderTargetTextures;

        Ogre::MultiRenderTarget* mMultiRenderTarget;
        Ogre::RenderTarget* mSingleRenderTarget;
    };

    typedef Ogre::SharedPtr<GPU2DOperation> GPU2DOperationPtr;
}

#endif // __GPU2DOPERATION_H__
