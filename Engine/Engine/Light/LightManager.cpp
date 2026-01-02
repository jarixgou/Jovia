#include "LightManager.hpp"

#include "Light.hpp"


namespace Engine
{
	std::vector<Light*> LightManager::m_lightList;

	void LightManager::Add(Light* _light)
	{
		m_lightList.push_back(_light);
	}

	void LightManager::Update()
	{
		for (auto& light : m_lightList)
		{

		}
	}

	void LightManager::Display()
	{
		
	}

	void LightManager::Cleanup()
	{
		for (auto& light : m_lightList)
		{
			delete light;
		}
		m_lightList.clear();
	}

	const std::vector<Light*>& LightManager::GetLightList()
	{
		return m_lightList;
	}
}