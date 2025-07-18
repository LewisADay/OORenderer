
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

# ASSIMP
add_subdirectory("assimp-6.0.2")
target_link_libraries(${PROJECT_NAME} PUBLIC "assimp::assimp")