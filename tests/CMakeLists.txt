## ORenderers test

set(TEST_EXE ${PROJECT_NAME}_TEST)

add_executable(${TEST_EXE}
	main.cpp
)

target_link_libraries(${TEST_EXE}
	${PROJECT_NAME}
)

target_include_directories(${TEST_EXE} PRIVATE
	${OORENDERER_INCLUDE_DIR}
)

# Copy our test resources to the binary location
add_custom_command(TARGET ${TEST_EXE}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources)
