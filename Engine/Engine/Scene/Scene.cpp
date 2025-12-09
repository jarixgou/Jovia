#include "Scene.hpp"

#include "../Asset/AssetsManager.hpp"
#include "../Render/RenderAPI.hpp"
#include "../Layer/LayerManager.hpp"

Engine::Scene::Scene()
{

}

Engine::Scene::~Scene()
{

}

void Engine::Scene::Init()
{

}

void Engine::Scene::PollEvents(sf::Event& _event)
{

}

void Engine::Scene::Update(float _dt)
{

}

void Engine::Scene::Display()
{

}

void Engine::Scene::Cleanup()
{
	Engine::LayerManager::Clear();
	Engine::AssetsManager::Clear();
	Engine::RenderAPI::Cleanup();
}