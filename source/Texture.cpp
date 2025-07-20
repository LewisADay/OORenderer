
#include "OORenderer/Texture.h"

#include <iostream>
#include <LoggingAD/LoggingAD.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace OORenderer {

	Texture::Texture(std::filesystem::path texturePath)
	{
		LoadTexture(texturePath);
	}

	Texture::Texture(const char* texturePath)
		: Texture(std::filesystem::path{ texturePath })
	{ }

	Texture::Texture(GLFWwindow* window)
	{
		BindToWindow(window);
	}

	Texture::Texture(GLFWwindow* window, std::filesystem::path texturePath)
	{
		BindToWindow(window);
		LoadTexture(texturePath);
	}

	Texture::Texture(GLFWwindow* window, const char* texturePath)
		: Texture(window, std::filesystem::path{ texturePath })
	{ }

	Texture::Texture(const Window& window)
		: Texture(window.GetGLFWWindow())
	{ }

	Texture::Texture(const Window& window, std::filesystem::path texturePath)
		: Texture(window.GetGLFWWindow(), texturePath)
	{ }

	Texture::Texture(const Window& window, const char* texturePath)
		: Texture(window, std::filesystem::path{texturePath})
	{ }

	Texture::~Texture() {
		if (m_RawData) {
			stbi_image_free(m_RawData);
		}
	}

	unsigned int Texture::GetTextureID() {
		return m_TextureID;
	}

	void Texture::LoadTexture(std::filesystem::path texturePath, const bool flip) {

		LoggingAD::Trace("[OORenderer::Texture::Load] Loading texture from path: {}", texturePath.string());

		// Needs improvement, this method I think only works for jpg/jpeg formats
		// TODO Investigate
		stbi_set_flip_vertically_on_load(flip);
		m_RawData = stbi_load(texturePath.string().c_str(), &m_Width, &m_Height, &m_NumChannels, 0);

		if (!m_RawData) {
			LoggingAD::Error("[OORenderer::Texture::Load] Error loading texture from path: {}", texturePath.string());
			return;
		}

		BindTexture();

		if (m_Window) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RawData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		UnbindTexture();

		m_TextureFilePath = texturePath;
	}

	void Texture::BindToWindow(GLFWwindow* window) {
		m_Window = window;

		m_OldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureWrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureFilteringMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureFilteringMag);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_BorderColour.data());

		// If we have data already loaded, push to this context
		if (m_RawData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RawData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, NULL);

		Window::ActivateGLFWWindow(m_OldContext);
	}

	void Texture::SetTextureWrapMode(GLint mode) {
		SetTextureWrapModeS(mode);
		SetTextureWrapModeT(mode);
	}

	void Texture::SetTextureWrapModeS(GLint mode) {
		BindTexture();

		m_TextureWrapS = mode;

		if (m_Window) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureWrapS);
		}

		UnbindTexture();
	}

	void Texture::SetTextureWrapModeT(GLint mode) {
		BindTexture();

		m_TextureWrapT = mode;

		if (m_Window) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureWrapT);
		}

		UnbindTexture();
	}

	void Texture::SetBorderColour(std::array<float, 4> colour) {
		BindTexture();

		m_BorderColour = colour;

		if (m_Window) {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_BorderColour.data());
		}

		UnbindTexture();
	}

	std::filesystem::path Texture::GetTexturePath() const {
		return m_TextureFilePath;
	}

	void Texture::BindTexture() {
		if (!m_Window) {
			return;
		}

		m_OldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::UnbindTexture() {
		if (!m_Window) {
			return;
		}

		glBindTexture(GL_TEXTURE_2D, NULL);
		Window::ActivateGLFWWindow(m_OldContext);
	}

} // OORenderer
