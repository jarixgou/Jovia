#include  "Game.hpp"

#include <execution>
#include <Engine/DrawableObject/DrawableObject.hpp>

#include <Engine/Asset/AssetsManager.hpp>
#include <Engine/Scene/ScenesManager.hpp>

#include <Engine/Layer/LayerManager.hpp>
#include <Engine/Camera/Camera.hpp>

#include <Engine/Interface/Camera/CameraInterface.hpp>

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
	m_tileSheet.setTexture(*spritesheetTexture);

	m_textureSliced = Engine::SliceTexture(*spritesheetTexture, { 32,32 });
	std::string message = "Sliced texture into " + std::to_string(m_textureSliced.size()) + " sprites.";
	LOG_DEBUG(message.c_str());

	m_camera = new Engine::Camera();
	m_camera->SetType(Engine::CameraType::ISOMETRIC);
	m_camera->SetPos({ 0.f,0.f,0.f });
	m_camera->SetSize({ 1920.f,1080.f});
	m_camera->SetZoom(1.f);
	m_camera->SetFree(false);

	mapVertex = new Engine::DrawableObject(sf::VertexArray(), sf::RenderStates());
	mapVertex->states.texture = spritesheetTexture;

	mapVertexBuffer = new Engine::DrawableObject(sf::VertexArray(), sf::RenderStates());
	mapVertexBuffer->states.texture = spritesheetTexture;

	m_tileSprites.reserve(m_textureSliced.size());
	for (int i = 0; i < m_textureSliced.size(); ++i)
	{
		sf::Sprite spr(*spritesheetTexture, m_textureSliced[i].rect);
		m_tileSprites.emplace_back(spr);
	}

	for (int y = 0; y < 500; ++y)
	{
		for (int x = 0; x < 500; ++x)
		{
			map[y][x] = rand() % m_textureSliced.size();
		}
	}

	// ✅ Créer les vertices avec le bon ordre pour les quads
	for (int y = 0; y < 500; ++y)
	{
		for (int x = 0; x < 500; ++x)
		{
			sf::IntRect rect = m_textureSliced[map[y][x]].rect;

			float worldX = static_cast<float>(x);
			float worldY = static_cast<float>(y);

			// ✅ Ordre correct pour sf::Quads (sens horaire ou anti-horaire cohérent)
			// Top-Left
			mapVertex->shape.append({
				{worldX, worldY},
				sf::Color::White,
				{static_cast<float>(rect.left), static_cast<float>(rect.top)}
				});

			// Top-Right
			mapVertex->shape.append({
				{worldX + 1.f, worldY},
				sf::Color::White,
				{static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top)}
				});

			// Bottom-Right
			mapVertex->shape.append({
				{worldX + 1.f, worldY + 1.f},
				sf::Color::White,
				{static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top + rect.height)}
				});

			// Bottom-Left
			mapVertex->shape.append({
				{worldX, worldY + 1.f},
				sf::Color::White,
				{static_cast<float>(rect.left), static_cast<float>(rect.top + rect.height)}
				});
		}
	}
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{

}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	Engine::LayerManager::Clear();

	// ✅ Vérifier si la caméra a changé
	sf::Vector3f currentPos = m_camera->GetPos();
	float currentZoom = m_camera->GetZoom();

	bool cameraMoved = (currentPos.x != m_lastCameraPos.x) ||
		(currentPos.y != m_lastCameraPos.y) ||
		(currentPos.z != m_lastCameraPos.z) ||
		(currentZoom != m_lastCameraZoom);

	// ✅ Ne recalculer QUE si la caméra a bougé
	if (cameraMoved)
	{
		sf::FloatRect visibleArea = m_camera->GetVisibleArea({ 32,32 });

		int startX = std::max(0, static_cast<int>(visibleArea.left));
		int endX = std::min(500, static_cast<int>((visibleArea.left + visibleArea.width)) + 1);
		int startY = std::max(0, static_cast<int>(visibleArea.top));
		int endY = std::min(500, static_cast<int>((visibleArea.top + visibleArea.height)) + 1);

		int estimatedTiles = (endX - startX) * (endY - startY);

		mapVertexBuffer->shape.clear();
		mapVertexBuffer->shape.setPrimitiveType(sf::Quads);

		const float scaledTileX = 32.f * currentZoom;
		const float scaledTileY = scaledTileX;

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				const sf::IntRect& rect = m_textureSliced[map[y][x]].rect;

				const sf::Vector2f screenPos = m_camera->WorldToScreen(
					{ static_cast<float>(x), static_cast<float>(y), 0.f },
					{ 32.f, 32.f }
				);

				// Top-Left
				mapVertexBuffer->shape.append({
					screenPos,
					sf::Color::White,
					{static_cast<float>(rect.left), static_cast<float>(rect.top)}
					});

				// Top-Right
				mapVertexBuffer->shape.append({
					{screenPos.x + scaledTileX, screenPos.y},
					sf::Color::White,
					{static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top)}
					});

				// Bottom-Right
				mapVertexBuffer->shape.append({
					{screenPos.x + scaledTileX, screenPos.y + scaledTileY},
					sf::Color::White,
					{static_cast<float>(rect.left + rect.width), static_cast<float>(rect.top + rect.height)}
					});

				// Bottom-Left
				mapVertexBuffer->shape.append({
					{screenPos.x, screenPos.y + scaledTileY},
					sf::Color::White,
					{static_cast<float>(rect.left), static_cast<float>(rect.top + rect.height)}
					});
			}
		}

		// ✅ Mettre à jour le cache
		m_lastCameraPos = currentPos;
		m_lastCameraZoom = currentZoom;
	}

	// ✅ Toujours dessiner (même si pas recalculé)
	Engine::LayerManager::Reserve(1);
	Engine::LayerManager::Update(m_camera->GetType());
	Engine::LayerManager::Add(mapVertexBuffer, { 0.f,0.f,0.f }, { 32,32 }, -1000);

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
