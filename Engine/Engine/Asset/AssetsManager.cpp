#include "AssetsManager.hpp"

namespace Engine
{
	std::vector<std::unique_ptr<Asset>> AssetsManager::m_assetsList;

	const std::vector<std::unique_ptr<Asset>>& AssetsManager::GetList()
	{
		return m_assetsList;
	}

	void AssetsManager::UnLoad(const char* _name)
	{
		for (auto& asset : m_assetsList)
		{
			if (asset->m_name == _name)
			{
				asset->Unload();
				return;
			}
		}
	}

	void AssetsManager::Remove(const char* _name)
	{
		for (auto it = m_assetsList.begin(); it != m_assetsList.end(); ++it)
		{
			if ((*it)->m_name == _name)
			{
				(*it)->Unload();
				m_assetsList.erase(it);

				std::string message = "Asset removed: " + std::string(_name);
				LOG_INFO(message.c_str(), true);
				return;
			}
		}
	}

	void AssetsManager::ClearLoaded()
	{
		for (auto& asset : m_assetsList)
		{
			if (asset->m_load)
			{
				asset->Unload();
			}
		}
		LOG_INFO("All loaded assets unloaded", true);
	}

	void AssetsManager::Clear()
	{
		for (auto& asset : m_assetsList)
		{
			asset->Unload();
		}
		m_assetsList.clear();

		LOG_INFO("All assets unloaded", true);
	}
}