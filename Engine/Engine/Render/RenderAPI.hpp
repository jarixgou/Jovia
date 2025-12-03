#ifndef RENDER_API__HPP
#define RENDER_API__HPP

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Engine
{
	class RenderAPI
	{
	private:
		static bool m_used;
		static sf::RenderStates m_renderPipline;
		static sf::Shader* m_lightShader;

		static std::unique_ptr<sf::Sprite> m_lightMapSprite;
		static std::unique_ptr<sf::Sprite> m_sceneMapSprite;
	public: // Public variable
		static std::unique_ptr<sf::RenderTexture> m_lightMap;
		static std::unique_ptr<sf::RenderTexture> m_sceneMap;
	public: // Public function
		static void Init();
		static void Clear();
		static void Display();
		static void Cleanup();
		static bool GetIsUsed();
	};

}
#endif