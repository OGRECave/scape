/**
 * ScapeEngine::HeightfieldGeomTile class
 *
 * Representation of the actual renderable geometry, derived from MovableObject
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDGEOMTILE_H__
#define __HEIGHTFIELDGEOMTILE_H__

namespace ScapeEngine
{

	class HeightfieldGeomTile : public Ogre::SimpleRenderable
	{
	public:
		HeightfieldGeomTile(
			class HeightfieldGeom* heightfieldGeom, 
			const Ogre::Rect& visibleQuadRect, 
			size_t bufferedQuadColumnCount, 
			size_t bufferQuadRowCount,
			const string& name = Utils::emptyString);


		~HeightfieldGeomTile();

		/** Returns the type name of this object. */
		virtual const string& getMovableType() const {return mMovableType;}

		/** Retrieves the radius of the origin-centered bounding sphere 
		 	 for this object.
		*/
		virtual Ogre::Real getBoundingRadius() const {return mBoundingRadius;}

        /** Retrieves a weak reference to the material this renderable object uses.
        @remarks
            Note that the Renderable also has the option to override the getTechnique method
            to specify a particular Technique to use instead of the best one available.
        */
		virtual const Ogre::MaterialPtr& getMaterial() const;

		/** Returns the camera-relative squared depth of this renderable.
		@remarks
			Used to sort transparent objects. Squared depth is used rather than
			actual depth to avoid having to perform a square root on the result.
		*/
		virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;

        /** Update a custom GpuProgramParameters constant which is derived from 
            information only this Renderable knows.
        @remarks
            This method allows a Renderable to map in a custom GPU program parameter
            based on it's own data. This is represented by a GPU auto parameter
            of ACT_CUSTOM, and to allow there to be more than one of these per
            Renderable, the 'data' field on the auto parameter will identify
            which parameter is being updated. The implementation of this method
            must identify the parameter being updated, and call a 'setConstant' 
            method on the passed in GpuProgramParameters object, using the details
            provided in the incoming auto constant setting to identify the index
            at which to set the parameter.
        @par
            You do not need to override this method if you're using the standard
            sets of data associated with the Renderable as provided by setCustomParameter
            and getCustomParameter. By default, the implementation will map from the
            value indexed by the 'constantEntry.data' parameter to a value previously
            set by setCustomParameter. But custom Renderables are free to override
            this if they want, in any case.
        @param constantEntry The auto constant entry referring to the parameter
            being updated
        @param params The parameters object which this method should call to 
            set the updated parameters.
        */
        virtual void _updateCustomGpuParameter(
			const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry,
			Ogre::GpuProgramParameters* params) const;

		/** Internal method to notify the object of the camera to be used for the next rendering operation.
            @remarks
                Certain objects may want to do specific processing based on the camera position. This method notifies
                them incase they wish to do this.
        */
		virtual void _notifyCurrentCamera(Ogre::Camera* cam);

		inline void markDirtyData() {mDirtyData = true;}

		inline void markDirtyDataNeighbor() {mDirtyDataNeighbor = true;}

		inline void markDirtyMaterial() {mDirtyMaterial = true;}

		inline bool isDirtyData() const {return mDirtyData;}

		inline bool isDirtyDataNeighbor() const {return mDirtyDataNeighbor;}

		inline bool isDirtyMaterial() const {return mDirtyMaterial;}

		inline HeightfieldGeom* getHeightfieldGeom() const {return mHeightfieldGeom;}

		inline Ogre::Rect getVisibleQuadRect() const {return mVisibleQuadRect;}

		inline int getTemplateQuadColumnCount() const {return mTemplateQuadColumnCount;}

		inline int getTemplateQuadRowCount() const {return mTemplateQuadRowCount;}

		int needsLODUpdate() const;

		int getCurrentLODSpacing() const;

		void updateLOD();

		void postFrameUpdate();

		void addNeighbor(HeightfieldGeomTile* neighborTile);

		const std::list<HeightfieldGeomTile*>& getNeighbors() {return mNeighborTiles;}

	private:
		static const string mMovableType;

		class HeightfieldGeom* mHeightfieldGeom;

		bool mDirtyData;

		bool mDirtyDataNeighbor;

		bool mDirtyMaterial;

		Ogre::Rect mVisibleQuadRect;

		int mTemplateQuadColumnCount;

		int mTemplateQuadRowCount;

		int mQuadColumnSpacing;

		int mQuadRowSpacing;

		Ogre::Real mBoundingRadius;

		class ShaderCustomAutoConstants* mGeomTileShaderCustomAutoConstants;

		Ogre::Real mFrameLeastSquaredPixelsWorldQuad;

		void revalidate();

		void updateRenderOperation();

		void updateRenderIndexBuffer();

		int getBestLODSpacing() const;

		unsigned long mLastMillisRevalidate;

		std::list<HeightfieldGeomTile*> mNeighborTiles;

	};

}

#endif // __HEIGHTFIELDGEOMTILE_H__