
set(VENDOR_DIR "vendor")

# GLAD
set(GLAD_DIR "GLAD")
set(GLAD_DIR "${VENDOR_DIR}/${GLAD_DIR}" PARENT_SCOPE)
target_sources(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/src/glad.c")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include")

# GLFW
set(GLFW_DIR "glfw-3.4")
set(GLFW_DIR "${VENDOR_DIR}/${GLFW_DIR}" PARENT_SCOPE)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_DIR}/include")

# STB Image
target_include_directories(${PROJECT_NAME} PUBLIC "stb_image")

# GLM
target_include_directories(${PROJECT_NAME} PUBLIC "glm-1.0.1")


# Submodules #
# ASSIMP
add_subdirectory("assimp")
target_link_libraries(${PROJECT_NAME} PUBLIC "assimp::assimp")

# LoggingAD
# N.B: To disable entirely set compile time flag LOGGING_AD_DISABLE, for example on dist builds
set(LOGGINGAD_DIR "LoggingAD")
set(LOGGINGAD_DIR "${VENDOR_DIR}/${LOGGINGAD_DIR}" PARENT_SCOPE)
set(LOGGINGAD_BUILD_EXAMPLES OFF)
add_subdirectory(${LOGGINGAD_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LOGGINGAD_LIBRARY_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${LOGGINGAD_INCLUDE_DIR})
