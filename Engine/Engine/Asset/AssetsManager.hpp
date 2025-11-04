#ifndef ASSETS_MANAGER__HPP
#define ASSETS_MANAGER__HPP

#include <memory>
#include <vector>

#include "../Logger/Logger.hpp"
#include "Asset.hpp"

namespace Engine
{
	class AssetsManager
	{
	private:
		static std::vector<std::unique_ptr<Asset>> m_assetsList;
	public:
		template <class T>
		static bool Add(const char* _path)
		{
			if (_path == nullptr || std::string(_path).empty())
			{
				LOG_WARNING("Invalid asset path");
				return false;
			}

			for (auto& asset : m_assetsList)
			{
				if (asset->m_path == _path)
				{
					std::string message = "Asset with path: " + std::string(_path) + " already exists.";
					LOG_WARNING(message.c_str());
					return false;
				}
			}

			std::unique_ptr<Asset> asset = nullptr;

			using AssetType = typename AssetTypeMap<T>::Type;
			std::string assetType = typeid(AssetType).name();
			try
			{
				if (std::is_base_of_v<Asset, AssetType>)
				{
					asset = std::make_unique<AssetType>();
				}
				else
				{
					std::string message = "Unknown asset type: " + assetType;
					LOG_WARNING(message.c_str());
					return false;
				}
			}
			catch (const std::bad_alloc& e)
			{
				std::string message = "Failed to allocate memory for " + assetType + " asset reason: " + e.what();
				LOG_CRITICAL(message.c_str());
				return false;
			}
			catch (const std::exception& e)
			{
				std::string message = "Failed to add asset '" + std::string(_path) + "' reason: " + e.what();
				LOG_CRITICAL(message.c_str());
				return false;
			}

			auto& [load, path, name] = *asset;
			path = _path;

			name = std::string(_path).substr(std::string(_path).find_last_of("/\\") + 1);
			size_t lastDot = name.find_last_of('.');
			if (lastDot != std::string::npos)
			{
				name = name.substr(0, lastDot);
			}

			load = false;
			m_assetsList.push_back(std::move(asset));
			std::string message = "Asset added: " + name;
			LOG_INFO(message.c_str());
			return true;
		}

		template <class T>
		static const T* Get(const char* _name)
		{
			if (_name == nullptr || std::string(_name).empty())
			{
				LOG_WARNING("Invalid asset name");
				return nullptr;
			}

			for (auto& asset : m_assetsList)
			{
				auto& [load, path, name] = *asset;

				if (name == _name)
				{
					using AssetType = typename AssetTypeMap<T>::Type;

					AssetType* data = dynamic_cast<AssetType*>(asset.get());
					if (data == nullptr)
					{
						std::string message = "Asset with name: " + name + " is not of type: " + std::string(typeid(T).name());
						LOG_WARNING(message.c_str());
						return nullptr;
					}

					if (!load)
					{
						if (asset->LoadFromFile())
						{
							load = true;
							std::string message = "Loaded asset: " + name;
							LOG_INFO(message.c_str());
						}
						else
						{
							std::string message = "Failed to load " + asset->GetResourceTypeName() + " from file :" + path;
							LOG_ERROR(message.c_str());
							return nullptr;
						}
					}

					std::string message = "Asset found: " + name;
					LOG_DEBUG(message.c_str());
					return static_cast<const T*>(asset->GetResource());
				}
			}

			std::string message = "Asset not found: " + std::string(_name);
			LOG_ERROR(message.c_str());
			return nullptr;
		}

		static const std::vector<std::unique_ptr<Asset>>& GetList();

		static void UnLoad(const char* _name);
		static void Remove(const char* _name);

		static void ClearLoaded();
		static void Clear();
	};
}

#endif
