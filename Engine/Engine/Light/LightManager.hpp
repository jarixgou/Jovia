#ifndef LIGHT_MANAGER__HPP
#define LIGHT_MANAGER__HPP
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>

namespace Engine
{
	class Camera;
	class Light;

	class LightManager
	{
	private:
		static std::vector<Light*> m_lightList;
	public:
		static void Add(Light* _light);
		static void Update();
		static void Display();
		static void Cleanup();
		static const std::vector<Light*>& GetLightList();
	};
}

#endif // !LIGHT_MANAGER__HPP