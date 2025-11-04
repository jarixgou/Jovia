#ifndef ASSET__HPP
#define ASSET__HPP

#include <memory>
#include <string>

#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace Engine
{
	template<typename ResourceType>
	struct AssetTypeMap;

	class Asset
	{
	public:
		virtual ~Asset() = default;

		bool m_load;

		std::string m_path;
		std::string m_name;

		virtual bool LoadFromFile() = 0;
		virtual const void* GetResource() const = 0;
		virtual std::string GetResourceTypeName() const = 0;
		virtual void Unload() = 0;
	};

	class TextureAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Texture> m_texture;

		bool LoadFromFile() override;
		const void* GetResource() const override;
		std::string GetResourceTypeName() const override;
		void Unload() override;
	};

	class SoundAsset : public Asset
	{
	public:
		std::unique_ptr<sf::SoundBuffer> m_sound;

		bool LoadFromFile() override;
		const void* GetResource() const override;
		std::string GetResourceTypeName() const override;
		void Unload() override;
	};

	class MusicAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Music> m_music;

		bool LoadFromFile() override;
		const void* GetResource() const override;
		std::string GetResourceTypeName() const override;
		void Unload() override;
	};

	class FontAsset : public Asset
	{
	public:
		std::unique_ptr<sf::Font> m_font;

		bool LoadFromFile() override;
		const void* GetResource() const override;
		std::string GetResourceTypeName() const override;
		void Unload() override;
	};

	template <>
	struct AssetTypeMap<sf::Texture>
	{
		using Type = TextureAsset;
	};

	template <>
	struct AssetTypeMap<sf::SoundBuffer>
	{
		using Type = SoundAsset;
	};

	template <>
	struct AssetTypeMap<sf::Music>
	{
		using Type = MusicAsset;
	};

	template <>
	struct AssetTypeMap<sf::Font>
	{
		using Type = FontAsset;
	};
}

#endif