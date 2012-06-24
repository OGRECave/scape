/**
 * ScapeEngine::InputPointer class
 *
 * Mouse/tablet 2D/3D pointer input representator
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __INPUTPOINTER_H__
#define __INPUTPOINTER_H__


#include "InputPointer.h"


namespace ScapeEngine
{
	class RenderView;
	class HeightfieldGeom;

	class InputPointer
	{
	public:
		class PointerState
		{
			friend class InputPointer;

		public:
			PointerState(int x, int y, Ogre::Real pressure) 
			:	mWindow2DPoint(x, y), 
				mPressure(pressure),
				mIs3DPointDirty(true),
				mIsFocusedRenderViewDirty(true),
				mFocusedRenderView(NULL),
				m3DPointGeom(NULL) {}

			const IVector2& getWindow2DPoint() const {return mWindow2DPoint;}
			std::pair<RenderView*, Ogre::Vector2> getRenderView2DPoint() const;
			HeightfieldGeom* get3DPointGeom() const;
			const Ogre::Vector3& get3DPoint() const;
			const Ogre::Real getPressure() const {return mPressure;}

		private:
			IVector2 mWindow2DPoint;
			Ogre::Real mPressure;
			mutable RenderView* mFocusedRenderView;
			mutable Ogre::Vector2 mFocusedRenderView2DPoint;
			mutable Ogre::Vector3 m3DPoint;
			mutable HeightfieldGeom* m3DPointGeom;
			mutable bool mIs3DPointDirty;
			mutable bool mIsFocusedRenderViewDirty;
		};

		typedef std::list<PointerState> FrameStateTrailHistory;

		InputPointer(class InputManager* inputManager);

		PointerState& getCurrentState() {return mCurrentState;}

		void update();

		void startNewFrame();

		const FrameStateTrailHistory& getFrameHistory() {return mFrameStateTrailHistory;}

	private:

		class InputManager* mInputManager;

		PointerState mCurrentState;

		FrameStateTrailHistory mFrameStateTrailHistory;
	};
}

#endif // __INPUTPOINTER_H__