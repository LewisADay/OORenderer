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
