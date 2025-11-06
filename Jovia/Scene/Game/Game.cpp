#include  "Game.hpp"

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Camera/Camera.hpp>

#include "Engine/Interface/Camera/CameraInterface.hpp"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/spritesheet.png");

	const sf::Texture* spritesheetTexture = Engine::AssetsManager::Get<sf::Texture>("spritesheet");
	tileSheet.setTexture(*spritesheetTexture);

	textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str());

	m_camera = new Engine::Camera();
	m_camera->SetType(Engine::CameraType::ISOMETRIC);
	m_camera->SetPos({ 0.f,0.f,0.f });
	m_camera->SetSize({ 1920.f,1080.f});
	m_camera->SetZoom(1.f);
	m_camera->SetFree(false);

	for (int y = 0; y < 50; ++y)
	{
		for (int x = 0; x < 50; ++x)
		{
			map[y][x] = rand() % textureSliced.size();
		}
	}
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{

}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	m_camera->Update(_dt);
	Engine::CameraInterface::Update(m_camera);
}

void Game::Display(sf::RenderWindow& _window)
{
	for (int y = 0; y < 50; ++y)
	{
		for (int x = 0; x < 50; ++x)
		{
			tileSheet.setTextureRect(textureSliced[map[y][x]].rect);
			m_camera->DrawObject(tileSheet, { static_cast<float>(x), static_cast<float>(y), 3 }, { 32.f, 32.f }, _window);
		}
	}
	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
