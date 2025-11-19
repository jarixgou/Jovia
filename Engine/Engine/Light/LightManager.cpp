#include "LightManager.hpp"

#include "Light.hpp"

namespace Engine
{
	std::vector<std::unique_ptr<Light>> LightManager::m_lightList;

	void LightManager::Add(std::unique_ptr<Light>& _light)
	{
		m_lightList.push_back(std::move(_light));
	}

	void LightManager::Update()
	{

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