/**
 * ScapeEngine::EngineInterface class
 *
 * Engine interface to other projects. All communication with the engine should go through this interface.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __ENGINEINTERFACE_H__
#define __ENGINEINTERFACE_H__

//#include "UIElementTypes.h"
/*
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
*/

#define ENGINE_API

#define MATERIALTOOLNAME _T("Material")


namespace ScapeEngine 
{
    typedef std::list<std::string> StringList;
    typedef std::map<std::string, std::string> StringStringMap;
    typedef std::map<std::string, StringList> StringStringListMap;
    typedef std::map<std::string, std::pair<std::string, std::string>> StringStringStringPairMap;

	// ----------------------------------------------------------------------------
	enum EScapeUIElementGroupId
	{
		SCAPEUIELEMENTGROUPID_OPERATION,
		SCAPEUIELEMENTGROUPID_MATERIAL,
		SCAPEUIELEMENTGROUPID_SKY,
		SCAPEUIELEMENTGROUPID_RENDERWINDOW,
		SCAPEUIELEMENTGROUPID_FILEEXPORT,
		SCAPEUIELEMENTGROUPID_FILEIMPORT,
	};


	class UIElementContainer;

	class ENGINE_API EngineInterface
	{

	public:

		// Create engine instance. Should be called only once per applicatin run.
		void initialize();

		// Destruct the entire engine. 
		void deinitialize();

		// Attach keyboard and mouse input to the given (root) window
		void attachInputToWindow(std::string inputWindow);

		// Create a new renderview, embedded in the windowHandle window
		void createRenderView(int viewId, const std::string &windowHandle, int left, int top, int width, int height);

		// Call this when parent render window is moved or resized
		void onRenderViewMovedOrResized(int viewId, int left, int top, int width, int height);

		// Detach embedded renderview 
		void detachRenderView(int viewId);

		// Give the engine a change to update itself and render a frame
		// if it wants to. Call this as very often.
		void update();

		// Open text console, redirecting all standard outputs to it
		static void openConsole();

		// Give engine handle a chance to react to a fatal (unhandled) exception
		static bool onFatalException();

		void onRenderViewSetFocus(int viewId);

		void onRenderViewKillFocus(int viewId);

        void writeAuxiliaryFile(const std::string& fileName, const void* data, int bytes);
        std::pair<void*, size_t> readAuxiliaryFile(const std::string& fileName);

		StringList			getUIElementNameList(EScapeUIElementGroupId groupId);
        StringList			getUIElementPropertyNameList(EScapeUIElementGroupId groupId, const std::string& element);

        std::string				getUIElementPresetPropertyValue(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& presetName, const std::string& propertyName);
        void				setUIElementPresetPropertyValue(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& presetName, const std::string& propertyName, const std::string& value);

		StringStringStringPairMap getFileFilterMap(EScapeUIElementGroupId groupId);

        bool				selectOperation(const std::string& toolName);

        bool				exportImageFile(const std::string& encoderName, const std::string& fileName, std::string* error);
        bool				importImageFile(const std::string& decoderName, const std::string& fileName, std::string* error);
        std::string				getDecoderNameFromExtension(const std::string& fileName);

        std::string				getUIElementPropertyField(const std::string& elementName, const std::string& propertyName, const std::string& propertyFieldName);

        std::string				getUIElementPropertyValue(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& propertyName);
        std::string				setUIElementPropertyValue(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& propertyName, const std::string& value);

        StringStringMap		getUIElementPropertyValueMap(EScapeUIElementGroupId groupId, const std::string& elementName);
        StringStringMap		setUIElementPropertyValueMap(EScapeUIElementGroupId groupId, const std::string& elementName, const StringStringMap& valueMap);

        StringList			getUIElementPresetPropertyNames(EScapeUIElementGroupId groupId, const std::string& elementName);

        StringStringMap		getUIElementPresetPropertyValueMap(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& presetName);
        void				setUIElementPresetPropertyValueMap(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& presetName, const StringStringMap& valueMap);

        void				deleteUIElementPreset(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& presetName);
        std::string				importUIElementPreset(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& fileName);
        void				exportUIElementPreset(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& fileName);

        std::string				makeUniquePresetName(EScapeUIElementGroupId groupId, const std::string& elementName, const std::string& baseName);

	protected:
        UIElementContainer* getUIElementContainer(EScapeUIElementGroupId scapeGroupId, const std::string& elementName);
	};
}

#endif // __ENGINEINTERFACE_H__
