#ifndef ASSET__HPP
#define ASSET__HPP

#include <memory>
#include <string>
#include <SFML/Graphics/Shader.hpp>

#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace Engine
{
	/**
	 * @brief Template structure for mapping resource types to asset classes
	 *
	 * Used by the asset manager to determine which asset class handles
	 * a specific SFML resource type.
	 *
	 * @tparam ResourceType SFML resource type (sf::Texture, sf::Font, etc.)
	 */
	template<typename ResourceType>
	struct AssetTypeMap;

	/**
	 * @class Asset
	 * @brief Abstract base class for all loadable game assets
	 *
	 * Provides a common interface for loading, managing, and unloading
	 * different types of game resources (textures, sounds, fonts, etc.).
	 * All assets support encrypted file loading through the ChangeFileFormat system.
	 */
	class Asset
	{
	public:
		/**
		 * @brief Virtual destructor
		 */
		virtual ~Asset() = default;

		bool m_load;        ///< Flag indicating if the asset is currently loaded
		std::string m_path; ///< File path to the asset
		std::string m_name; ///< Unique identifier name for the asset

		/**
		 * @brief Loads the asset from file
		 *
		 * Pure virtual function that handles decryption and loading logic
		 * specific to each asset type.
		 *
		 * @return True if loading succeeded, false otherwise
		 */
		virtual bool LoadFromFile() = 0;

		/**
		 * @brief Gets a pointer to the underlying resource
		 *
		 * @return Const void pointer to the loaded resource
		 */
		virtual const void* GetResource() const = 0;

		/**
		 * @brief Gets the type name of the resource
		 *
		 * @return String representing the resource type (e.g., "Texture", "Sound")
		 */
		virtual std::string GetResourceTypeName() const = 0;

		/**
		 * @brief Unloads the asset and frees memory
		 */
		virtual void Unload() = 0;
	};

	/**
	 * @class TextureAsset
	 * @brief Asset class for SFML textures
	 *
	 * Handles loading and management of image files (PNG, JPG, etc.)
	 * as SFML textures. Supports encrypted asset files.
	 */
	class TextureAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Texture> m_texture; ///< Managed texture resource

		/**
		 * @brief Loads texture from encrypted file
		 *
		 * @return True if loading succeeded, false otherwise
		 */
		bool LoadFromFile() override;

		/**
		 * @brief Gets pointer to the texture
		 *
		 * @return Const void pointer to sf::Texture
		 */
		const void* GetResource() const override;

		/**
		 * @brief Gets the resource type name
		 *
		 * @return "Texture"
		 */
		std::string GetResourceTypeName() const override;

		/**
		 * @brief Unloads the texture and frees GPU memory
		 */
		void Unload() override;
	};

	/**
	 * @class SoundAsset
	 * @brief Asset class for SFML sound buffers
	 *
	 * Handles loading and management of sound effect files (WAV, OGG, etc.)
	 * as SFML sound buffers. Supports encrypted asset files.
	 */
	class SoundAsset : public Asset
	{
	public:
		std::unique_ptr<sf::SoundBuffer> m_sound; ///< Managed sound buffer resource

		/**
		 * @brief Loads sound from encrypted file
		 *
		 * @return True if loading succeeded, false otherwise
		 */
		bool LoadFromFile() override;

		/**
		 * @brief Gets pointer to the sound buffer
		 *
		 * @return Const void pointer to sf::SoundBuffer
		 */
		const void* GetResource() const override;

		/**
		 * @brief Gets the resource type name
		 *
		 * @return "Sound"
		 */
		std::string GetResourceTypeName() const override;

		/**
		 * @brief Unloads the sound buffer and frees memory
		 */
		void Unload() override;
	};

	/**
	 * @class MusicAsset
	 * @brief Asset class for SFML music streams
	 *
	 * Handles loading and management of music files (OGG, FLAC, WAV, etc.)
	 * as streaming SFML music. Supports encrypted asset files.
	 */
	class MusicAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Music> m_music; ///< Managed music stream resource

		/**
		 * @brief Opens music stream from encrypted file
		 *
		 * @return True if opening succeeded, false otherwise
		 */
		bool LoadFromFile() override;

		/**
		 * @brief Gets pointer to the music stream
		 *
		 * @return Const void pointer to sf::Music
		 */
		const void* GetResource() const override;

		/**
		 * @brief Gets the resource type name
		 *
		 * @return "Music"
		 */
		std::string GetResourceTypeName() const override;

		/**
		 * @brief Stops and unloads the music stream
		 */
		void Unload() override;
	};

	/**
	 * @class FontAsset
	 * @brief Asset class for SFML fonts
	 *
	 * Handles loading and management of font files (TTF, OTF, etc.)
	 * as SFML fonts. Supports encrypted asset files.
	 */
	class FontAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Font> m_font; ///< Managed font resource

		/**
		 * @brief Loads font from encrypted file
		 *
		 * @return True if loading succeeded, false otherwise
		 */
		bool LoadFromFile() override;

		/**
		 * @brief Gets pointer to the font
		 *
		 * @return Const void pointer to sf::Font
		 */
		const void* GetResource() const override;

		/**
		 * @brief Gets the resource type name
		 *
		 * @return "Font"
		 */
		std::string GetResourceTypeName() const override;

		/**
		 * @brief Unloads the font and frees memory
		 */
		void Unload() override;
	};

	/**
	 * @class ShaderAsset
	 * @brief Asset class for SFML shaders
	 *
	 * Handles loading and management of GLSL shader files (.vert, .frag, .geom)
	 * as SFML shaders. Determines shader type from file extension.
	 */
	class ShaderAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Shader> m_shader; ///< Managed shader resource

		/**
		 * @brief Loads shader from file
		 *
		 * Determines shader type (vertex/fragment/geometry) from file extension
		 * and compiles the shader code.
		 *
		 * @return True if loading and compilation succeeded, false otherwise
		 */
		bool LoadFromFile() override;

		/**
		 * @brief Gets pointer to the shader
		 *
		 * @return Const void pointer to sf::Shader
		 */
		const void* GetResource() const override;

		/**
		 * @brief Gets the resource type name
		 *
		 * @return "Shader"
		 */
		std::string GetResourceTypeName() const override;

		/**
		 * @brief Unloads the shader and frees GPU resources
		 */
		void Unload() override;
	};

	/**
	 * @brief Maps sf::Texture to TextureAsset
	 */
	template <>
	struct AssetTypeMap<sf::Texture>
	{
		using Type = TextureAsset;
	};

	/**
	 * @brief Maps sf::SoundBuffer to SoundAsset
	 */
	template <>
	struct AssetTypeMap<sf::SoundBuffer>
	{
		using Type = SoundAsset;
	};

	/**
	 * @brief Maps sf::Music to MusicAsset
	 */
	template <>
	struct AssetTypeMap<sf::Music>
	{
		using Type = MusicAsset;
	};

	/**
	 * @brief Maps sf::Font to FontAsset
	 */
	template <>
	struct AssetTypeMap<sf::Font>
	{
		using Type = FontAsset;
	};

	/**
	 * @brief Maps sf::Shader to ShaderAsset
	 */
	template <>
	struct AssetTypeMap<sf::Shader>
	{
		using Type = ShaderAsset;
	};
}

#endif