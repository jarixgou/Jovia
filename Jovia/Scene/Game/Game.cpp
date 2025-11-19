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

	light = new Engine::Light;
	light->SetPos({ 0,0,0, });
	light->SetRadius(250);
	lightCircle.setRadius(light->GetRadius());
	lightCircle.setOrigin({ 500 / 2, 500 / 2 });
	lightCircle.setTexture(Engine::AssetsManager::Get<sf::Texture>("Light"));

	colliderTest.setPrimitiveType(sf::Quads);
	colliderTest.append(sf::Vertex({SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2}, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT  / 2}, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT / 2 + 100}, sf::Color::Red));
	colliderTest.append(sf::Vertex({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 + 100 }, sf::Color::Red));
	g_shadowQuads.setPrimitiveType(sf::Quads);

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
	lightCircle.setPosition({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });


	// --- Calcul des arêtes silhouette et génération des quads d'ombre ---
	{
		auto computeSignedArea = [](const sf::VertexArray& poly) -> float {
			float area = 0.f;
			size_t n = poly.getVertexCount();
			for (size_t i = 0; i < n; ++i)
			{
				const sf::Vector2f& a = poly[i].position;
				const sf::Vector2f& b = poly[(i + 1) % n].position;
				area += (a.x * b.y - b.x * a.y);
			}
			return 0.5f * area;
			};

		auto normalize = [](const sf::Vector2f& v) -> sf::Vector2f {
			const float len = std::sqrt(v.x * v.x + v.y * v.y);
			if (len == 0.f) return sf::Vector2f{ 0.f, 0.f };
			return sf::Vector2f{ v.x / len, v.y / len };
			};

		const sf::Vector2f lightPos = lightCircle.getPosition();
		const float extrude = 2000.f; // distance d'extrusion (assez grande pour projeter loin)

		// vide l'ancienne géométrie d'ombre
		g_shadowQuads.clear();

		// aire signée pour déterminer l'orientation (sens horaire/anti-horaire)
		const float signedArea = computeSignedArea(colliderTest);

		const size_t vertexCount = colliderTest.getVertexCount();
		if (vertexCount >= 2)
		{
			for (size_t i = 0; i < vertexCount; ++i)
			{
				sf::Vector2f v1 = colliderTest[i].position;
				sf::Vector2f v2 = colliderTest[(i + 1) % vertexCount].position;

				sf::Vector2f edge = v2 - v1;
				sf::Vector2f toLight = lightPos - v1;

				// cross(edge, toLight) (z-component)
				const float cross = edge.x * toLight.y - edge.y * toLight.x;

				// si polygon CCW (signedArea > 0) alors cross < 0 => arête face à la lumière
				// si polygon CW  (signedArea < 0) alors cross > 0 => arête face à la lumière
				const bool facesLight = (signedArea >= 0.f) ? (cross < 0.f) : (cross > 0.f);

				if (facesLight)
				{
					// projeter chaque sommet loin depuis la lumière
					sf::Vector2f dir1 = normalize(v1 - lightPos);
					sf::Vector2f dir2 = normalize(v2 - lightPos);

					sf::Vector2f p1 = v1 + dir1 * extrude;
					sf::Vector2f p2 = v2 + dir2 * extrude;

					// couleur d'ombre : opaque près de l'objet, transparent loin (fade)
					const sf::Color nearColor(0, 255, 0, 255);
					const sf::Color farColor(0, 255, 0, 255);

					// quad : v1, v2, p2, p1 (remarque : ordre important pour affichage correct)
					g_shadowQuads.append(sf::Vertex(v1, nearColor));
					g_shadowQuads.append(sf::Vertex(v2, nearColor));
					g_shadowQuads.append(sf::Vertex(p2, farColor));
					g_shadowQuads.append(sf::Vertex(p1, farColor));
				}
			}
		}
	}
	// --- fin génération d'ombre ---


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

	if (g_shadowQuads.getVertexCount() > 0)
	{
		_window.draw(g_shadowQuads);
	}

	_window.draw(lightCircle);
	_window.draw(colliderTest);

	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
