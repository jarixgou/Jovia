#ifndef LIGHT_MANAGER__HPP
#define LIGHT_MANAGER__HPP
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Engine
{
	class Light;

	class LightManager
	{
	private:
		static std::vector<std::unique_ptr<Light>> m_lightList;
		static std::unique_ptr<sf::RenderTexture> m_lightMap;
		static std::unique_ptr<sf::RenderTexture> m_shadowMap;
	public:
		static void Add(std::unique_ptr<Light>& _light);
		static void Update();
		static void Cleanup();
		static const std::vector<std::unique_ptr<Light>>& GetLightList();
	};
}

#endif // !LIGHT_MANAGER__HPP