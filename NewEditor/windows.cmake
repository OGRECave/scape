# Ogre DLLs
if(EXISTS ${OGRE_Overlay_BINARY_REL})
    install(FILES ${OGRE_Overlay_BINARY_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D11_REL})
    install(FILES ${OGRE_RenderSystem_Direct3D11_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_D3D11_REL "#")
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D9_REL})
    install(FILES ${OGRE_RenderSystem_Direct3D9_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_D3D9_REL "#")
endif()
if(EXISTS ${OGRE_RenderSystem_GL3Plus_REL})
    install(FILES ${OGRE_RenderSystem_GL3Plus_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_GL3PLUS_REL "#")
endif()
if(EXISTS ${OGRE_RenderSystem_GL_REL})
    install(FILES ${OGRE_RenderSystem_GL_REL} DESTINATION bin CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_GL_REL "#")
endif()
install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
    ${OGRE_Plugin_OctreeSceneManager_REL}
    ${OGRE_PLUGIN_DIR_REL}/cg.dll
    ${OGRE_PLUGIN_DIR_REL}/OIS.dll
    ${OGRE_Plugin_CgProgramManager_REL}
    ${OGRE_Plugin_ParticleFX_REL}
    ${OGRE_Terrain_BINARY_REL}
    ${OGRE_Paging_BINARY_REL}
    DESTINATION bin
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel)

if(EXISTS ${OGRE_Overlay_BINARY_DBG})
    install(FILES ${OGRE_Overlay_BINARY_DBG} DESTINATION bin CONFIGURATIONS Debug)
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D11_DBG})
    install(FILES ${OGRE_RenderSystem_Direct3D11_DBG} DESTINATION bin CONFIGURATIONS Debug)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_D3D11_DBG "#")
endif()
if(EXISTS ${OGRE_RenderSystem_Direct3D9_DBG})
    install(FILES ${OGRE_RenderSystem_Direct3D9_DBG} DESTINATION bin CONFIGURATIONS Debug)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_D3D9_DBG "#")
endif()
if(EXISTS ${OGRE_RenderSystem_GL3Plus_DBG})
    install(FILES ${OGRE_RenderSystem_GL3Plus_DBG} DESTINATION bin CONFIGURATIONS Debug)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_GL3PLUS_DBG "#")
endif()
if(EXISTS ${OGRE_RenderSystem_GL_DBG})
    install(FILES ${OGRE_RenderSystem_GL_DBG} DESTINATION bin CONFIGURATIONS Debug)
else()
    set(OGRE_COMMENT_RENDERSYSTEM_GL_DBG "#")
endif()
install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
    ${OGRE_Plugin_OctreeSceneManager_DBG}
    ${OGRE_PLUGIN_DIR_DBG}/cg.dll
    ${OGRE_PLUGIN_DIR_DBG}/OIS_d.dll
    ${OGRE_Plugin_CgProgramManager_DBG}
    ${OGRE_Plugin_ParticleFX_DBG}
    ${OGRE_Terrain_BINARY_DBG}
    ${OGRE_Paging_BINARY_DBG}
    DESTINATION bin
    CONFIGURATIONS Debug)

# Qt5 DLLs
file(TO_CMAKE_PATH $ENV{QTDIR} QTDIR) 
set(QT_DLL_DIR "${QTDIR}/bin")
set(QT_PLUGINS_DIR "${QTDIR}/plugins")
install(FILES
    ${QT_DLL_DIR}/Qt5Core.dll
    ${QT_DLL_DIR}/Qt5Gui.dll
    ${QT_DLL_DIR}/Qt5Svg.dll
    ${QT_DLL_DIR}/Qt5Widgets.dll
	${QT_DLL_DIR}/Qt5CLucene.dll
    DESTINATION bin
    CONFIGURATIONS Release)
install(FILES
    ${QT_DLL_DIR}/Qt5Cored.dll
    ${QT_DLL_DIR}/Qt5Guid.dll
    ${QT_DLL_DIR}/Qt5Svgd.dll
    ${QT_DLL_DIR}/Qt5Widgetsd.dll
	${QT_DLL_DIR}/Qt5CLucened.dll
    DESTINATION bin
    CONFIGURATIONS Debug)
install(FILES ${QT_PLUGINS_DIR}/iconengines/qsvgicon.dll
    DESTINATION bin/iconengines
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
install(FILES ${QT_PLUGINS_DIR}/iconengines/qsvgicond.dll
    DESTINATION bin/iconengines
    CONFIGURATIONS Debug)
install(FILES ${QT_PLUGINS_DIR}/platforms/qwindows.dll
    DESTINATION bin/platforms
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
install(FILES ${QT_PLUGINS_DIR}/platforms/qwindowsd.dll
    DESTINATION bin/platforms
    CONFIGURATIONS Debug)
