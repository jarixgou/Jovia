#include  "Game.hpp"

#include <corecrt_math_defines.h>
#include <execution>
#include <Engine/DrawableObject/DrawableObject.hpp>
#include <Engine/Chunk/ChunkManager.hpp>

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Layer/LayerManager.hpp>
#include <Engine/Camera/Camera.hpp>

#include <Engine/Interface/Camera/CameraInterface.hpp>

#include <Engine/Chunk/Chunk.hpp>

#include <Engine/Light/Light.hpp>
#include <Engine/Light/LightManager.hpp>

#include "Engine/Render/RenderAPI.hpp"

void Game::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/Tile.png");
	Engine::AssetsManager::Add<sf::Texture>("Assets/Light.png");

	Engine::RenderAPI::Init();

	const sf::Texture* spritesheetTexture = Engine::AssetsManager::Get<sf::Texture>("Tile");
	m_tileSheet.setTexture(*spritesheetTexture);

	m_textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(m_textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str(), false);

	m_camera = new Engine::Camera();
	m_camera->SetType(Engine::CameraType::ISOMETRIC);
	m_camera->SetPos({ 0.f,0.f,-0.5f });
	m_camera->SetSize({ 1920.f,1080.f });
	m_camera->SetFree(false);

	Engine::Light* light = new Engine::Light(&lightCircle);
	light->SetPos({ 0,0,0, });
	light->SetRadius(500);
	light->SetColor(sf::Color::Cyan);
	light->SetIntensity(1.5f);
	light->SetAngularFalloff(1.f);
	light->SetRadialFalloff(1.f);
	light->SetVolumetricIntensity(1.f);

	lightList.push_back(light);

	Engine::Light* light2 = new Engine::Light(&lightCircle);
	light2->SetPos({ SCREEN_WIDTH / 2 - 100,SCREEN_HEIGHT / 2,0, });
	light2->SetRadius(500);
	light2->SetColor(sf::Color::White);
	light2->SetIntensity(1.f);
	light2->SetAngularFalloff(1.f);
	light2->SetRadialFalloff(1.f);
	light2->SetVolumetricIntensity(1.f);

	lightList.push_back(light2);

	lightCircle.setTexture(Engine::AssetsManager::Get<sf::Texture>("Light"));


	colliderTest.setPrimitiveType(sf::Quads);
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT / 2 }, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT / 2 + 100 }, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 + 100 }, sf::Color::Red));

	colliderTest2.setPrimitiveType(sf::Quads);
	float cosAngle = cosf(M_PI / 4);
	float sinAngle = sinf(M_PI / 4);
	sf::Vector2f pos = { SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 600 };
	colliderTest2.append(sf::Vertex(
		{ pos.x * cosAngle - pos.y * sinAngle,
			 pos.x * sinAngle + pos.y * cosAngle }
	, sf::Color::Blue));
	colliderTest2.append(sf::Vertex(
		{ (pos.x + 100) * cosAngle - pos.y * sinAngle,
			 (pos.x + 100) * sinAngle + pos.y * cosAngle },
		sf::Color::Blue));
	colliderTest2.append(sf::Vertex(
		{ (pos.x + 100) * cosAngle - (pos.y + 100) * sinAngle,
			 (pos.x + 100) * sinAngle + (pos.y + 100) * cosAngle },
		sf::Color::Blue));
	colliderTest2.append(sf::Vertex(
		{ pos.x * cosAngle - (pos.y + 100) * sinAngle,
			 pos.x * sinAngle + (pos.y + 100) * cosAngle },
		sf::Color::Blue));

	m_chunkManager = new Engine::ChunkManager();

	m_renderStates.texture = spritesheetTexture;
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

	sf::Vector2i mousePos = sf::Mouse::getPosition(_renderWindow);
	Engine::Light* light = lightList[0];
	light->SetPos({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y),0 });

	for (auto& light : lightList)
	{
		light->Update();
		light->BuildProjectedShadow(colliderTest);
		light->BuildProjectedShadow(colliderTest2);
	}

	m_chunkManager->UpdateVisibleChunks(m_camera);
	m_chunkManager->RebuildDirtyChunks(m_camera);

	Engine::CameraInterface::Update(m_camera);
	m_camera->Update(_dt);
}

void Game::Display(sf::RenderWindow& _window)
{
	Engine::RenderAPI::Clear();
	Engine::LayerManager::Draw(m_camera, _window);

	for (auto& light : lightList)
	{
		light->Display();
		Engine::RenderAPI::m_lightMap->draw(light->GetCompositeLightAndShadow(), sf::BlendAdd);
	}

	for (const auto& chunk : m_chunkManager->GetChunks())
	{
		Engine::RenderAPI::m_sceneMap->draw(chunk->GetGroundVertices(), m_renderStates);
		Engine::RenderAPI::m_sceneMap->draw(chunk->GetObjectVertices(), m_renderStates);
	}

	Engine::RenderAPI::m_sceneMap->draw(colliderTest);
	Engine::RenderAPI::m_sceneMap->draw(colliderTest2);

	Engine::RenderAPI::Display(_window);
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
