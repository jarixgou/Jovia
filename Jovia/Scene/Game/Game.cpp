#include  "Game.hpp"

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init()
{
	
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{
	
}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	
}

void Game::Display(sf::RenderWindow& _window)
{
	
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
