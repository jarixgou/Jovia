#include  "Game.hpp"

#include <execution>
#include <Engine/DrawableObject/DrawableObject.hpp>
#include <Engine/Chunk/ChunkManager.hpp>

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Layer/LayerManager.hpp>
#include <Engine/Camera/Camera.hpp>

#include <Engine/Interface/Camera/CameraInterface.hpp>

#include <Engine/Chunk/Chunk.hpp>

void Game::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/spritesheet.png");

	const sf::Texture* spritesheetTexture = Engine::AssetsManager::Get<sf::Texture>("spritesheet");
	m_tileSheet.setTexture(*spritesheetTexture);

	m_textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(m_textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str());

	m_camera = new Engine::Camera();
	m_camera->SetType(Engine::CameraType::ISOMETRIC);
	m_camera->SetPos({ 0.f,0.f,0.f });
	m_camera->SetSize({ 1920.f,1080.f });
	m_camera->SetZoom(1.f);
	m_camera->SetFree(false);

	m_renderStates.texture = spritesheetTexture;

	m_chunkManager = new Engine::ChunkManager();

	std::vector<sf::IntRect> textureRects;
	for (const auto& slice : m_textureSliced)
	{
		textureRects.push_back(slice.rect);
	}
	m_chunkManager->Init({ 32, 32 }, textureRects);

	for (int y = 0; y < 32 * 16; ++y)
	{
		for (int x = 0; x < 32 * 16; ++x)
		{
			const int id = rand() % m_textureSliced.size();
			m_chunkManager->SetTileAt({ x, y }, id, 0);
		}
	}
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{

}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	Engine::LayerManager::Clear();

	m_chunkManager->UpdateVisibleChunks(m_camera);
	m_chunkManager->RebuildDirtyChunks(m_camera);

	Engine::CameraInterface::Update(m_camera);
	m_camera->Update(_dt);
}

void Game::Display(sf::RenderWindow& _window)
{
	Engine::LayerManager::Draw(m_camera, _window);

	for (const auto& chunk : m_chunkManager->GetChunks())
	{
		_window.draw(chunk->GetGroundVertices(), m_renderStates);
		_window.draw(chunk->GetObjectVertices(), m_renderStates);
	}

	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
