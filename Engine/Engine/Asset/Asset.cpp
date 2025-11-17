#include "Asset.hpp"

#include "../Logger/Logger.hpp"

namespace Engine
{
	bool TextureAsset::LoadFromFile()
	{
		try
		{
			m_texture = std::make_unique<sf::Texture>();
			m_texture->loadFromFile(m_path);
			m_texture->generateMipmap();
			return true;
		}
		catch (std::bad_alloc&)
		{
			std::string message = "Failed to allocate memory for " + GetResourceTypeName() + " : " + m_path;
			LOG_CRITICAL(message.c_str());
			return false;
		}
	}

	const void* TextureAsset::GetResource() const
	{
		return m_texture.get();
	}

	std::string TextureAsset::GetResourceTypeName() const
	{
		return "Texture";
	}

	void TextureAsset::Unload()
	{
		if (m_texture == nullptr)
		{
			return;
		}

		m_load = false;
		m_texture.reset();

		std::string message = "Asset '" + m_name + "' unloaded.";
		LOG_INFO(message.c_str());
	}

	bool SoundAsset::LoadFromFile()
	{
		try
		{
			m_sound = std::make_unique<sf::SoundBuffer>();
			return m_sound->loadFromFile(m_path);
		}
		catch (std::bad_alloc&)
		{
			std::string message = "Failed to allocate memory for " + GetResourceTypeName() + " : " + m_path;
			LOG_CRITICAL(message.c_str());
			return false;
		}
	}

	const void* SoundAsset::GetResource() const
	{
		return m_sound.get();
	}

	std::string SoundAsset::GetResourceTypeName() const
	{
		return "Sound";
	}

	void SoundAsset::Unload()
	{
		if (m_sound == nullptr)
		{
			return;
		}

		m_load = false;
		m_sound.reset();

		std::string message = "Asset '" + m_name + "' unloaded.";
		LOG_INFO(message.c_str());
	}

	bool MusicAsset::LoadFromFile()
	{
		try
		{
			m_music = std::make_unique<sf::Music>();
			return m_music->openFromFile(m_path);
		}
		catch (std::bad_alloc&)
		{
			std::string message = "Failed to allocate memory for " + GetResourceTypeName() + " : " + m_path;
			LOG_CRITICAL(message.c_str());
			return false;
		}
	}

	const void* MusicAsset::GetResource() const
	{
		return m_music.get();
	}

	std::string MusicAsset::GetResourceTypeName() const
	{
		return "Music";
	}

	void MusicAsset::Unload()
	{
		if (m_music == nullptr)
		{
			return;
		}

		m_load = false;
		m_music.reset();

		std::string message = "Asset '" + m_name + "' unloaded.";
		LOG_INFO(message.c_str());
	}

	bool FontAsset::LoadFromFile()
	{
		try
		{
			m_font = std::make_unique<sf::Font>();
			return m_font->loadFromFile(m_path);
		}
		catch (std::bad_alloc&)
		{
			std::string message = "Failed to allocate memory for " + GetResourceTypeName() + " : " + m_path;
			LOG_CRITICAL(message.c_str());
			return false;
		}
	}

	const void* FontAsset::GetResource() const
	{
		return m_font.get();
	}

	std::string FontAsset::GetResourceTypeName() const
	{
		return "Font";
	}

	void FontAsset::Unload()
	{
		if (m_font == nullptr)
		{
			return;
		}

		m_load = false;
		m_font.reset();

		std::string message = "Asset '" + m_name + "' unloaded.";
		LOG_INFO(message.c_str());
	}
}