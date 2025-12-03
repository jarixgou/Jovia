#ifndef SCENES_MANAGER__HPP
#define SCENES_MANAGER__HPP

#include <iostream>
#include <unordered_map>
#include <SFML/Window.hpp>

#include "Scene.hpp"
#include "../Logger/Logger.hpp"

namespace Engine
{
	class ScenesManager
	{
	private:
		static std::shared_ptr<Scene> m_currentScene;
		static std::string m_currentSceneName;
		static std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenesList;
	public:
		ScenesManager() = default;
		~ScenesManager() = default;

		static void PollEvents(sf::Event& _event);
		static void Update(float _dt);
		static void Display();
		static void Cleanup();

		template<class T>
		static bool CreateScene(const char* _sceneName)
		{
			if (!std::is_base_of_v<Scene, T>)
			{
				std::string message = std::string(typeid(T).name()) + "must inherit scene";
				LOG_WARNING(message.c_str(), true);
				return false;
			}

			if (m_scenesList.contains(_sceneName))
			{
				std::string message = "Scene " + std::string(_sceneName) + " already exists.";
				LOG_WARNING(message.c_str(), true);
				return false;
			}

			try
			{
				const auto scene = std::make_shared<T>();
				auto [iter, inserted] = m_scenesList.emplace(std::string(_sceneName), scene);
				if (inserted)
				{
					std::string message = "Scene " + std::string(_sceneName) + " created.";
					LOG_INFO(message.c_str(), true);
					return true;
				}
				else
				{
					std::string message = "Scene " + std::string(_sceneName) + " already exists.";
					LOG_WARNING(message.c_str(), true);
					return false;
				}
			}
			catch (const std::bad_alloc& e)
			{
				std::string message = std::string("Failed to allocate scene '") + _sceneName + "' reason: " + e.what();
				LOG_CRITICAL(message.c_str(), true);
				return false;
			}
			catch (const std::exception& e)
			{
				std::string message = "Failed to create scene '" + std::string(_sceneName) + "' reason: " + e.what();
				LOG_CRITICAL(message.c_str(), true);
				return false;
			}

			return false;
		}

		static bool LoadScene(const char* _sceneName);

		static bool HasScene(const char* _sceneName);
	};
}

#endif