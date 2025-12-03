#include "ScenesManager.hpp"

std::shared_ptr<Engine::Scene> Engine::ScenesManager::m_currentScene = nullptr;
std::string Engine::ScenesManager::m_currentSceneName;
std::unordered_map<std::string, std::shared_ptr<Engine::Scene>> Engine::ScenesManager::m_scenesList;

void Engine::ScenesManager::PollEvents(sf::Event& _event)
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->PollEvents(_event);
	}
}

void Engine::ScenesManager::Update(float _dt)
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Update(_dt);
	}
}

void Engine::ScenesManager::Display()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Display();
	}
}

void Engine::ScenesManager::Cleanup()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Cleanup();
	}
}

bool Engine::ScenesManager::LoadScene(const char* _sceneName)
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Cleanup();
	}

	auto it = m_scenesList.find(_sceneName);
	if (it != m_scenesList.end())
	{
		auto& [name, scene] = *it;

		m_currentScene = scene;
		m_currentScene->Init();

		m_currentSceneName = name;

		std::string message = "Scene " + std::string(_sceneName) + " loaded";
		LOG_INFO(message.c_str(), true);
		return true;
	}
	else
	{
		std::string message = "Scene " + std::string(_sceneName) + " not found";
		LOG_ERROR(message.c_str(), true);
		return false;
	}

	return false;
}

bool Engine::ScenesManager::HasScene(const char* _sceneName)
{

	auto it = m_scenesList.find(_sceneName);
	if (it != m_scenesList.end())
	{
		std::string message = "Scene '" + std::string(_sceneName) + "' find";
		LOG_INFO(message.c_str(), true);
		return true;
	}
	else
	{
		std::string message = "Scene '" + std::string(_sceneName) + "' not found";
		LOG_WARNING(message.c_str(), true);
		return false;
	}

	return false;
}
