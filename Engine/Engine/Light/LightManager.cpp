#include "LightManager.hpp"

#include "Light.hpp"
#include "../Camera/Camera.hpp"
#include "../Render/RenderAPI.hpp"

namespace Engine
{
	std::vector<std::unique_ptr<Light>> LightManager::m_lightList;

	void LightManager::Add(std::unique_ptr<Light>& _light)
	{
		m_lightList.push_back(std::move(_light));
	}

	void LightManager::Update(Camera* _camera)
	{
		for (auto & light : m_lightList)
		{

		}
	}

	void LightManager::Display()
	{
		for (auto& light : m_lightList)
		{
			if (RenderAPI::GetIsUsed())
			{
				light->Display();
				RenderAPI::m_lightMap->draw(light->GetCompositeLightAndShadow(), sf::BlendAdd);
			}
		}
	}

	void LightManager::Cleanup()
	{
		m_lightList.clear();
	}

	const std::vector<std::unique_ptr<Light>>& LightManager::GetLightList()
	{
		return m_lightList;
	}
}