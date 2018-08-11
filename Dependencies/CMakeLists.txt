file(GLOB EditorMedia_FILES "EditorMedia/*")
file(COPY ${EditorMedia_FILES} DESTINATION "${CMAKE_BINARY_DIR}/bin")
