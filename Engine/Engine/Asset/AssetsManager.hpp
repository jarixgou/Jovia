#ifndef ASSETS_MANAGER__HPP
#define ASSETS_MANAGER__HPP

#include <memory>
#include <vector>

#include "../Logger/Logger.hpp"
#include "Asset.hpp"

namespace Engine
{
	/**
	 * @class AssetsManager
	 * @brief Centralized manager for loading and accessing game assets
	 * 
	 * Provides static methods to add, retrieve, and manage assets throughout
	 * the application lifetime. Supports lazy loading and automatic type mapping.
	 * All assets are stored in a central list and accessed by name.
	 */
	class AssetsManager
	{
	private:
		static std::vector<std::unique_ptr<Asset>> m_assetsList; ///< Collection of all registered assets
	public:
		/**
		 * @brief Adds a new asset to the manager
		 * 
		 * Registers an asset with the given path. The asset is not loaded immediately
		 * but will be loaded on first access (lazy loading). Automatically extracts
		 * the asset name from the file path.
		 * 
		 * @tparam T SFML resource type (sf::Texture, sf::Font, etc.)
		 * @param _path File path to the asset (encrypted or plain)
		 * @return True if asset was successfully added, false if path is invalid or asset already exists
		 */
		template <typename T>
		static bool Add(const char* _path)
		{
			if (_path == nullptr || std::string(_path).empty())
			{
				LOG_WARNING("Invalid asset path", true);
				return false;
			}

			for (auto& asset : m_assetsList)
			{
				if (asset->m_path == _path)
				{
					std::string message = "Asset with path: " + std::string(_path) + " already exists.";
					LOG_WARNING(message.c_str(), true);
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
					LOG_WARNING(message.c_str(), true);
					return false;
				}
			}
			catch (const std::bad_alloc& e)
			{
				std::string message = "Failed to allocate memory for " + assetType + " asset reason: " + e.what();
				LOG_CRITICAL(message.c_str(), true);
				return false;
			}
			catch (const std::exception& e)
			{
				std::string message = "Failed to add asset '" + std::string(_path) + "' reason: " + e.what();
				LOG_CRITICAL(message.c_str(), true);
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
			LOG_INFO(message.c_str(), true);
			return true;
		}

		/**
		 * @brief Retrieves an asset by name
		 * 
		 * Searches for an asset by its name and returns a pointer to the loaded resource.
		 * If the asset hasn't been loaded yet, it will be loaded on first access (lazy loading).
		 * 
		 * @tparam T SFML resource type expected (sf::Texture, sf::Font, etc.)
		 * @param _name Name of the asset (filename without extension)
		 * @return Const pointer to the resource, or nullptr if not found or type mismatch
		 */
		template <typename T>
		static const T* Get(const char* _name)
		{
			if (_name == nullptr || std::string(_name).empty())
			{
				LOG_WARNING("Invalid asset name", true);
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
						LOG_WARNING(message.c_str(), true);
						return nullptr;
					}

					if (!load)
					{
						if (asset->LoadFromFile())
						{
							load = true;
							std::string message = "Loaded asset: " + name;
							LOG_INFO(message.c_str(), true);
						}
						else
						{
							std::string message = "Failed to load " + asset->GetResourceTypeName() + " from file :" + path;
							LOG_ERROR(message.c_str(), true);
							return nullptr;
						}
					}

					std::string message = "Asset found: " + name;
					LOG_DEBUG(message.c_str(), true);
					return static_cast<const T*>(asset->GetResource());
				}
			}

			std::string message = "Asset not found: " + std::string(_name);
			LOG_ERROR(message.c_str(), true);
			return nullptr;
		}

		/**
		 * @brief Gets the complete list of registered assets
		 * 
		 * @return Const reference to the vector of all assets
		 */
		static const std::vector<std::unique_ptr<Asset>>& GetList();

		/**
		 * @brief Unloads an asset from memory
		 * 
		 * Frees the memory used by the asset but keeps it registered.
		 * The asset can be reloaded on next access.
		 * 
		 * @param _name Name of the asset to unload
		 */
		static void UnLoad(const char* _name);

		/**
		 * @brief Removes an asset completely
		 * 
		 * Unloads and removes the asset from the manager.
		 * 
		 * @param _name Name of the asset to remove
		 */
		static void Remove(const char* _name);

		/**
		 * @brief Unloads all currently loaded assets
		 * 
		 * Frees memory for all loaded assets but keeps them registered.
		 */
		static void ClearLoaded();

		/**
		 * @brief Removes all assets from the manager
		 * 
		 * Unloads and removes all registered assets.
		 */
		static void Clear();
	};
}

#endif
