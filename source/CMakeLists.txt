
set(OORENDERER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OORENDERER_SOURCE_DIR ${OORENDERER_SOURCE_DIR} PARENT_SCOPE)

target_sources(${PROJECT_NAME} PRIVATE
	"Renderer.cpp"
	"Window.cpp"
	"ShaderProgram.cpp"
	"ShaderProgram_Uniforms.cpp"
	"Texture.cpp"
	"Camera.cpp"
	"Mesh.cpp"
	"Model.cpp"
	"RenderObject.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${OORENDERER_SOURCE_DIR}
	${OORENDERER_INCLUDE_DIR}
)
