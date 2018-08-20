/**
 * ScapeEngine::HeightfieldGeomMaterial class
 *
 * Base class of material managing class that can be used by HeightfieldGeoms and HeightfieldGeomTiles.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDGEOMMATERIAL_H__
#define __HEIGHTFIELDGEOMMATERIAL_H__

#include "EngineCore/UIElementClasses.h"

namespace ScapeEngine
{
	namespace _HeightfieldGeomMaterialNS
	{
		enum EShaderConstant
		{
			UNITQUADSCALE,
			UNITQUADBIAS,
			LOCALPOSSCALE,
			LOCALPOSBIAS,
			GRADIENTSCALEBIAS,
			BRUSHPOSRAMP,
			BRUSHCOLOR,
			SHADERCONSTANT_COUNT
		};
	}

	class HeightfieldGeomMaterial : public UIElementPresetContainerSimple
	{
		DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldGeomMaterial)
	public:

		HeightfieldGeomMaterial();

		virtual ~HeightfieldGeomMaterial();

		const Ogre::MaterialPtr& getMaterial();

		virtual void updateShaderConstantsGeom(const class HeightfieldGeom* heightfieldGeom, class ShaderCustomAutoConstants* shaderCustomAutoConstants, bool forceAll);
		virtual void updateShaderConstantsGeomTile(const class HeightfieldGeomTile* heightfieldGeomTile, class ShaderCustomAutoConstants* shaderCustomAutoConstants, bool forceAll);

		virtual void updateTextures() {}

		virtual void bindBrush(class HeightfieldBrush* brush);

		bool hasDirtySettings() {return mHasDirtySettings;}
		void clearDirtySettings() {mHasDirtySettings = false;}

		virtual Ogre::Real getLODPixelError() = 0;
		virtual Ogre::Real getLODDistancePower() = 0;

	protected:

		bool mHasDirtySettings;

		virtual Ogre::MaterialPtr createMaterial() = 0;

		void resetMaterial() {mMaterial.setNull();}

		virtual void bindShaderConstants();

		virtual bool initPersistentElementValueMap(StringStringMap& map) = 0;
		virtual bool initPersistentElementStringEnumMap(StringEnumMap& map) = 0;
		virtual string setUIElementPropertyValue(const string& elementName, const string& value) = 0;

	private:

		Ogre::MaterialPtr mMaterial;

		class HeightfieldBrush* mBrush;

		bool mInitialized;

		void initialize();
	};

}

#endif // __HEIGHTFIELDGEOMMATERIAL_H__