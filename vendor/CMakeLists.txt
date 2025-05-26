
set(VENDOR_DIR "vendor")

set(GLAD_DIR "GLAD")
set(GLAD_DIR "${VENDOR_DIR}/${GLAD_DIR}" PARENT_SCOPE)
add_subdirectory("${GLAD_DIR}")

set(GLFW_DIR "glfw-3.4")
set(GLFW_DIR "${VENDOR_DIR}/${GLFW_DIR}" PARENT_SCOPE)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_DIR}/include")
