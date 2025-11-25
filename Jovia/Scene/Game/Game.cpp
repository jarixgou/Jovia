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

void Game::Init()
{
	Engine::AssetsManager::Add<sf::Texture>("Assets/spritesheet.png");
	Engine::AssetsManager::Add<sf::Texture>("Assets/Light.png");

	const sf::Texture* spritesheetTexture = Engine::AssetsManager::Get<sf::Texture>("spritesheet");
	m_tileSheet.setTexture(*spritesheetTexture);

	m_textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(m_textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str());

	m_camera = new Engine::Camera();
	m_camera->SetType(Engine::CameraType::ISOMETRIC);
	m_camera->SetPos({ 0.f,0.f,-0.5f });
	m_camera->SetSize({ 1920.f,1080.f });
	m_camera->SetFree(false);

	Engine::Light* light = new Engine::Light(&lightCircle);
	light->SetPos({ 0,0,0, });
	light->SetRadius(500);
	light->SetColor(sf::Color::White);

	lightList.push_back(light);

	Engine::Light* light2 = new Engine::Light(&lightCircle);
	light2->SetPos({ SCREEN_WIDTH / 2 - 100,SCREEN_HEIGHT / 2,0, });
	light2->SetRadius(500);
	light2->SetColor(sf::Color::White);

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

	g_shadowQuads.setPrimitiveType(sf::Quads);

	m_compositeShadow.create(1920, 1080);
	m_compositeShadowSprite.setTexture(m_compositeShadow.getTexture());

	lightMapTexture.create(1920, 1080);
	lightMapSprite.setTexture(lightMapTexture.getTexture());

	fuseComposite.create(1920, 1080);
	compositeLightAndShadowSprite.setTexture(fuseComposite.getTexture());

	sceneTexture.create(1920, 1080);
	sceneSprite.setTexture(sceneTexture.getTexture());

	lightShader.loadFromFile("light.frag", sf::Shader::Type::Fragment);

	lightShaderStates.shader = &lightShader;
	lightShaderStates.blendMode = sf::BlendAlpha;

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

	lightPosCircle.setRadius(40);
	lightPosCircle.setOrigin(40, 40);
	lightPosCircle.setFillColor(sf::Color::Yellow);
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
	Engine::LayerManager::Draw(m_camera, _window);

	/*for (const auto& chunk : m_chunkManager->GetChunks())
	{
		_window.draw(chunk->GetGroundVertices(), m_renderStates);
		_window.draw(chunk->GetObjectVertices(), m_renderStates);
	}*/

	/*if (g_shadowQuads.getVertexCount() > 0)
	{
		_window.draw(g_shadowQuads);
	}*/


	fuseComposite.clear(sf::Color::Black);

	for (auto& light : lightList)
	{
		light->Display();
		fuseComposite.draw(light->GetCompositeLightAndShadow(), sf::BlendAdd);
	}

	fuseComposite.display();

	lightShader.setUniform("lightMap", fuseComposite.getTexture());

	sceneTexture.clear(sf::Color::Black);
	for (const auto& chunk : m_chunkManager->GetChunks())
	{
		sceneTexture.draw(chunk->GetGroundVertices(), m_renderStates);
		sceneTexture.draw(chunk->GetObjectVertices(), m_renderStates);
	}

	sceneTexture.draw(colliderTest);
	sceneTexture.draw(colliderTest2);

	sceneTexture.display();

	_window.draw(sceneSprite, lightShaderStates);

	for (auto& light : lightList)
	{
		lightPosCircle.setPosition({ light->GetPos().x, light->GetPos().y });
		_window.draw(lightPosCircle);
	}

	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
