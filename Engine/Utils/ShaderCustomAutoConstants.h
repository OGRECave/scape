/**
 * ScapeEngine::ShaderCustomAutoConstants class
 *
 * Helper class to ease the process of updating a shader program's custom constants
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __SHADERCUSTOMAUTOCONSTANTS_H__
#define __SHADERCUSTOMAUTOCONSTANTS_H__

namespace ScapeEngine
{
	class ShaderCustomAutoConstants
	{
	public:
		~ShaderCustomAutoConstants();

		void setConstant(size_t constantIndex, const Ogre::Real* data, int dataCount);
		void setConstant(size_t constantIndex, const Ogre::Vector4& data);
		void setConstant(size_t constantIndex, const Ogre::Matrix4& data);
		void clear();

		bool updateShaderParameter(Ogre::GpuProgramParameters* params, const Ogre::GpuProgramParameters::AutoConstantEntry& autoConstantEntry) const;

	private:
		struct AutoConstant
		{
			Utils::GUID guid;
			const Ogre::Real* data;
			int dataCount;
			bool isMatrix;

			AutoConstant() : guid(0), data(0), dataCount(0), isMatrix(false) {}
		};
		typedef std::vector<AutoConstant> AutoConstants;
		AutoConstants mAutoConstants;

		static std::vector<Utils::GUID> mGlobalActiveAutoConstantGUIDs;

		void setConstant(size_t constantIndex, const Ogre::Real* data, int dataCount, bool isMatrix);

	};
}

#endif // __SHADERCUSTOMAUTOCONSTANTS_H__
