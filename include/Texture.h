#pragma once

#include "Renderer.h"

#include <filesystem>
#include <array>

#include <Window.h>

namespace OORenderer {

	/// <summary>
	/// 2D Texture class
	/// </summary>
	class Texture {
	public:
		Texture(const Window& window);
		Texture(const Window& window, std::filesystem::path texturePath);
		Texture(const Window& window, const char* texturePath);
		~Texture();

		/// <summary>
		/// Return the OpenGL Texture ID for this texture
		/// </summary>
		/// <returns><OpenGL Texture ID for this texture/returns>
		unsigned int GetTextureID();

		/// <summary>
		/// Load the texture at the given path into this object
		/// </summary>
		/// <param name="texturePath">Path to texture to load</param>
		void LoadTexture(std::filesystem::path texturePath);

		/// <summary>
		/// Set the texture wrap mode (for both s and t)
		/// </summary>
		/// <param name="mode">OpenGL mode indicator, e.g. GL_MIRRORED_REPEAT</param>
		void SetTextureWrapMode(GLint mode);

		/// <summary>
		/// Set the texture wrap mode for s
		/// </summary>
		/// <param name="mode">OpenGL mode indicator, e.g. GL_MIRRORED_REPEAT</param>
		void SetTextureWrapModeS(GLint mode);

		/// <summary>
		/// Set the texture wrap mode for t
		/// </summary>
		/// <param name="mode">OpenGL mode indicator, e.g. GL_MIRRORED_REPEAT</param>
		void SetTextureWrapModeT(GLint mode);

		/// <summary>
		/// Set the texture border colour (only valid when using GL_CLAMP_TO_BORDER for texture wraping)
		/// </summary>
		/// <param name="colour">Colour to use as a border colour</param>
		void SetBorderColour(std::array<float, 4> colour);

	private: // Private methods
		void BindTexture();
		void UnbindTexture();

	private: // Private members
		GLFWwindow* m_Window;
		GLFWwindow* m_OldContext;
		unsigned int m_TextureID;

		unsigned char* m_RawData = nullptr;
		int m_Width = 0;
		int m_Height = 0;
		int m_NumChannels = 0;

		// Texture wrap details
		std::array<float, 4> m_BorderColour = { 1.0f, 1.0f, 1.0f, 1.0f };
		int m_TextureWrapS = GL_MIRRORED_REPEAT;
		int m_TextureWrapT = GL_MIRRORED_REPEAT;

		// Texture filtering details
		int m_TextureFilteringMin = GL_LINEAR_MIPMAP_LINEAR;
		int m_TextureFilteringMag = GL_LINEAR;
	};

} // OORenderer
