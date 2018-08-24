file(GLOB EditorMedia_FILES "EditorMedia/*")
file(COPY ${EditorMedia_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin")

file(COPY "${OGRE_CONFIG_DIR}/plugins.cfg" DESTINATION "${CMAKE_BINARY_DIR}/bin")
