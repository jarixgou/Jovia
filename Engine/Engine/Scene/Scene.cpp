#include "Scene.hpp"

#include "../Asset/AssetsManager.hpp"
#include "../Render/RenderAPI.hpp"
#include "../Layer/LayerManager.hpp"

#include "../Interface/Camera/CameraInterface.hpp"
#include "../Interface/Assets/AssetsInterface.hpp"
#include "../Interface/Log/LogInterface.hpp"
#include "../Interface/System/SystemInterface.hpp"
#include "../Interface/SceneExplorer/SceneExplorerInterface.hpp"

#include "../System/System.hpp"
#include "../Camera/Camera.hpp"

namespace Engine
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::Init()
	{

	}

	void Scene::PollEvents(sf::Event& _event)
	{

	}

	void Scene::Update()
	{
		AssetsInterface::Update();
		SystemInterface::Update();
		LogInterface::Update();
		SceneExplorerInterface::Update();
		CameraInterface::Update();

		if (*System::currentCamera != nullptr)
		{
			(*System::currentCamera)->Update();
		}
	}

	void Scene::Display()
	{

	}

	void Engine::Scene::Cleanup()
	{
		LayerManager::Clear();
		AssetsManager::Clear();
		RenderAPI::Cleanup();
	}
}