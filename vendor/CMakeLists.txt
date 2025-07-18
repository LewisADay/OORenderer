
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

# Submodules
# Ensure submodules are up to date
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

set(LOGGINGAD_DIR "LoggingAD")
set(LOGGINGAD_DIR "${VENDOR_DIR}/${LOGGINGAD_DIR}" PARENT_SCOPE)
add_subdirectory(${LOGGINGAD_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LOGGINGAD_LIBRARY_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${LOGGINGAD_INCLUDE_DIR})
