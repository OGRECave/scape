/**
 * ScapeEngine::GPU2DOperationQuad class
 *
 * 2D rectangle that can be added to a GPU2DOperation. Base class for specialized GPU2DOperationQuad subclasses
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __GPU2DOPERATIONQUAD_H__
#define __GPU2DOPERATIONQUAD_H__

namespace ScapeEngine
{
	class ShaderCustomAutoConstants;
	class GPU2DOperationRenderableQuad;

	class GPU2DOperationQuad 
    {
    public:

        GPU2DOperationQuad();
        virtual ~GPU2DOperationQuad();

		virtual void setRenderRect(const Ogre::Rect& rect);
		virtual const Ogre::Rect& getRenderRect();

		virtual void prepareForRender(Ogre::ushort zOrder);

		Ogre::MovableObject* getMovableObject();
		Ogre::Renderable* getRenderable();

	protected:
		GPU2DOperationRenderableQuad* mGPU2DOperationableQuad;
    };

	typedef Ogre::SharedPtr<GPU2DOperationQuad> GPU2DOperationQuadPtr;
}

#endif // __GPU2DOPERATIONQUAD_H__
