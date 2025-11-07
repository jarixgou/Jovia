#include  "Game.hpp"

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Layer/LayerManager.hpp>
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

	for (int y = 0; y < 500; ++y)
	{
		for (int x = 0; x < 500; ++x)
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
	Engine::LayerManager::Clear();

	// Frustum culling - ne dessiner que les tiles visibles
	sf::FloatRect visibleArea = m_camera->GetVisibleArea({ 32,32 });

	const float tileSize = 32.f;
	int startX = std::max(0, static_cast<int>(visibleArea.left));
	int endX = std::min(500, static_cast<int>((visibleArea.left + visibleArea.width)) + 1);
	int startY = std::max(0, static_cast<int>(visibleArea.top));
	int endY = std::min(500, static_cast<int>((visibleArea.top + visibleArea.height)) + 1);

	int tilesRendered = 0;
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			sf::Sprite spr = tileSheet;
			spr.setTextureRect(textureSliced[map[y][x]].rect);
			Engine::LayerManager::Add(spr, {static_cast<float>(x), static_cast<float>(y), 0}, {32, 32}, 0);
			tilesRendered++;
		}
	}

	Engine::LayerManager::Update(m_camera->GetType());

	Engine::CameraInterface::Update(m_camera);
	m_camera->Update(_dt);
}

void Game::Display(sf::RenderWindow& _window)
{
	Engine::LayerManager::Draw(m_camera, _window);
	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
