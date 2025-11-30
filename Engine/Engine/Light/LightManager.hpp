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
		static std::vector<std::unique_ptr<Light>> m_lightList;
	public:
		static void Add(std::unique_ptr<Light>& _light);
		static void Update(Camera* _camera);
		static void Display();
		static void Cleanup();
		static const std::vector<std::unique_ptr<Light>>& GetLightList();
	};
}

#endif // !LIGHT_MANAGER__HPP