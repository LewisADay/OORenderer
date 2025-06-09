
#include "Texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace OORenderer {

	Texture::Texture(const Window& window)
		: m_Window(window.GetGLFWWindow())
	{
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

		glBindTexture(GL_TEXTURE_2D, NULL);

		Window::ActivateGLFWWindow(m_OldContext);
	}

	Texture::Texture(const Window& window, std::filesystem::path texturePath)
		: Texture(window)
	{
		LoadTexture(texturePath);
	}

	Texture::Texture(const Window& window, const char* texturePath)
		: Texture(window, std::filesystem::path{texturePath})
	{ }

	Texture::~Texture() {
	}

	unsigned int Texture::GetTextureID() {
		return m_TextureID;
	}

	void Texture::LoadTexture(std::filesystem::path texturePath) {
		// Needs improvement, this method I think only works for jpg/jpeg formats
		// TODO Investigate
		m_RawData = stbi_load(texturePath.string().c_str(), &m_Width, &m_Height, &m_NumChannels, 0);

		if (!m_RawData) {
			std::cerr << "Error loading texture" << std::endl; // TODO LOGGING
			return;
		}

		BindTexture();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RawData);
		glGenerateMipmap(GL_TEXTURE_2D);

		UnbindTexture();

		stbi_image_free(m_RawData);
	}

	void Texture::SetTextureWrapMode(GLint mode) {
		SetTextureWrapModeS(mode);
		SetTextureWrapModeT(mode);
	}

	void Texture::SetTextureWrapModeS(GLint mode) {
		BindTexture();

		m_TextureWrapS = mode;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureWrapS);

		UnbindTexture();
	}

	void Texture::SetTextureWrapModeT(GLint mode) {
		BindTexture();

		m_TextureWrapT = mode;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureWrapT);

		UnbindTexture();
	}

	void Texture::SetBorderColour(std::array<float, 4> colour) {
		BindTexture();

		m_BorderColour = colour;
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_BorderColour.data());

		UnbindTexture();
	}

	void Texture::BindTexture() {
		m_OldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::UnbindTexture() {
		glBindTexture(GL_TEXTURE_2D, NULL);
		Window::ActivateGLFWWindow(m_OldContext);
	}

} // OORenderer
