#include "RenderAPI.hpp"

#include "../Asset/AssetsManager.hpp"
#include "../Logger/Logger.hpp"
#include "../System/System.hpp"

namespace Engine
{
	bool RenderAPI::m_used = false;
	sf::RenderStates RenderAPI::m_renderPipline;
	sf::Shader* RenderAPI::m_lightShader;
	std::unique_ptr<sf::Sprite> RenderAPI::m_lightMapSprite = nullptr;
	std::unique_ptr<sf::Sprite> RenderAPI::m_sceneMapSprite = nullptr;
	std::unique_ptr<sf::RenderTexture> RenderAPI::m_lightMap = nullptr;
	std::unique_ptr<sf::RenderTexture> RenderAPI::m_sceneMap = nullptr;

	void RenderAPI::Init()
	{
		if (m_used)
		{
			LOG_WARNING("RenderAPI is already used", true);
			return;
		}

		try
		{
			AssetsManager::Add<sf::Shader>("Assets/Shaders/LightShader.frag");
			AssetsManager::Add<sf::Shader>("Assets/Shaders/RenderPipeline.frag");
			m_lightShader = const_cast<sf::Shader*>(AssetsManager::Get<sf::Shader>("RenderPipeline"));

			m_used = true;
			System::drawLight = m_used;

			m_renderPipline.blendMode = sf::BlendAlpha;
			m_renderPipline.shader = m_lightShader;

			m_lightMap = std::make_unique<sf::RenderTexture>();
			m_sceneMap = std::make_unique<sf::RenderTexture>();
			m_lightMap->create(1920, 1080);
			m_sceneMap->create(1920, 1080);

			m_lightMapSprite = std::make_unique<sf::Sprite>(m_lightMap->getTexture());
			m_sceneMapSprite = std::make_unique<sf::Sprite>(m_sceneMap->getTexture());
		}
		catch (std::bad_alloc& e)
		{
			std::string message = "Failed to init RenderAPI reason: " + std::string(e.what());
			LOG_CRITICAL(message.c_str(), true);
		}
		catch (std::exception& e)
		{
			std::string message = "Failed to init RenderAPI reason: " + std::string(e.what());
			LOG_CRITICAL(message.c_str(), true);
		}
	}

	void RenderAPI::Clear()
	{
		if (m_lightMap != nullptr && m_sceneMap != nullptr && m_used)
		{
			m_lightMap->clear(sf::Color::Transparent);
			m_sceneMap->clear(sf::Color::Transparent);
		}
	}

	void RenderAPI::Display()
	{
		if (m_lightMap != nullptr && m_sceneMap != nullptr && m_used)
		{
			m_lightMap->display();
			m_sceneMap->display();
		}

		if (m_lightMapSprite != nullptr && m_sceneMapSprite != nullptr)
		{
			m_lightShader->setUniform("lightMap", m_lightMap->getTexture());

			System::drawCall += 1;
			System::window->draw(*m_sceneMapSprite, m_renderPipline);
		}
	}

	void RenderAPI::Cleanup()
	{
		m_used = false;

		m_lightShader = nullptr;
		delete m_lightShader;

		m_lightMap = nullptr;
		m_lightMap.reset();
		m_lightMapSprite = nullptr;
		m_lightMapSprite.reset();

		m_sceneMap = nullptr;
		m_sceneMap.reset();
		m_sceneMapSprite = nullptr;
		m_sceneMapSprite.reset();
	}

	bool RenderAPI::GetIsUsed()
	{
		return m_used;
	}
}
