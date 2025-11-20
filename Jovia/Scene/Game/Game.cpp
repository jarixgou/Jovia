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

	light = new Engine::Light;
	light->SetPos({ 0,0,0, });
	light->SetRadius(500);
	lightCircle.setRadius(light->GetRadius());
	lightCircle.setOrigin({ 500, 500 });
	lightCircle.setTexture(Engine::AssetsManager::Get<sf::Texture>("Light"));

	lightCircle2.setRadius(light->GetRadius());
	lightCircle2.setOrigin({ 500, 500 });
	lightCircle2.setTexture(Engine::AssetsManager::Get<sf::Texture>("Light"));
	lightCircle2.setPosition(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2);

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

	shadowMapTexture.create(1920, 1080);
	shadowMapSprite.setTexture(shadowMapTexture.getTexture());

	lightMapTexture.create(1920, 1080);
	lightMapSprite.setTexture(lightMapTexture.getTexture());

	compositeLightAndShadowTexture.create(1920, 1080);
	compositeLightAndShadowSprite.setTexture(compositeLightAndShadowTexture.getTexture());

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
}

void Game::PollEvents(sf::RenderWindow& _window, sf::Event& _event)
{

}

void Game::Update(sf::RenderWindow& _renderWindow, float _dt)
{
	Engine::LayerManager::Clear();

	sf::Vector2i mousePos = sf::Mouse::getPosition(_renderWindow);
	lightCircle.setPosition({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });

	// vide l'ancienne géométrie d'ombre
	g_shadowQuads.clear();

	bool canDraw = false;
	for (int i = 0; i < colliderTest.getVertexCount(); ++i)
	{
		sf::Vector2f pos = colliderTest[i].position;
		sf::Vector2f lightPos = lightCircle.getPosition();
		sf::Vector2f ap = { pos.x - lightPos.x, pos.y - lightPos.y };
		if (sqrtf((ap.x * ap.x) + (ap.y * ap.y)) < light->GetRadius())
		{
			canDraw = true;
		}
	}

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

		if (canDraw)
		{
			const sf::Vector2f lightPos = lightCircle.getPosition();
			const float extrude = 2000.f; // distance d'extrusion (assez grande pour projeter loin)

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
						const sf::Color nearColor(0, 0, 0, 255);
						const sf::Color farColor(0, 0, 0, 255);

						// quad : v1, v2, p2, p1 (remarque : ordre important pour affichage correct)
						g_shadowQuads.append(sf::Vertex(v1, nearColor));
						g_shadowQuads.append(sf::Vertex(v2, nearColor));
						g_shadowQuads.append(sf::Vertex(p2, farColor));
						g_shadowQuads.append(sf::Vertex(p1, farColor));
					}
				}
			}
		}
	}
	// --- fin génération d'ombre ---

	canDraw = false;
	for (int i = 0; i < colliderTest2.getVertexCount(); ++i)
	{
		sf::Vector2f pos = colliderTest2[i].position;
		sf::Vector2f lightPos = lightCircle.getPosition();
		sf::Vector2f ap = { pos.x - lightPos.x, pos.y - lightPos.y };
		if (sqrtf((ap.x * ap.x) + (ap.y * ap.y)) < light->GetRadius())
		{
			canDraw = true;
		}
	}

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

		if (canDraw)
		{
			const sf::Vector2f lightPos = lightCircle.getPosition();
			const float extrude = 2000.f; // distance d'extrusion (assez grande pour projeter loin)

			// aire signée pour déterminer l'orientation (sens horaire/anti-horaire)
			const float signedArea = computeSignedArea(colliderTest);

			const size_t vertexCount = colliderTest2.getVertexCount();
			if (vertexCount >= 2)
			{
				for (size_t i = 0; i < vertexCount; ++i)
				{
					sf::Vector2f v1 = colliderTest2[i].position;
					sf::Vector2f v2 = colliderTest2[(i + 1) % vertexCount].position;

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
						const sf::Color nearColor(0, 0, 0, 255);
						const sf::Color farColor(0, 0, 0, 255);

						// quad : v1, v2, p2, p1 (remarque : ordre important pour affichage correct)
						g_shadowQuads.append(sf::Vertex(v1, nearColor));
						g_shadowQuads.append(sf::Vertex(v2, nearColor));
						g_shadowQuads.append(sf::Vertex(p2, farColor));
						g_shadowQuads.append(sf::Vertex(p1, farColor));
					}
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

	/*if (g_shadowQuads.getVertexCount() > 0)
	{
		_window.draw(g_shadowQuads);
	}*/

	_window.draw(lightCircle);

	lightMapTexture.clear(sf::Color::Black);
	lightMapTexture.draw(lightCircle, sf::BlendAdd);
	lightMapTexture.draw(lightCircle2, sf::BlendAdd);
	lightMapTexture.display();

	shadowMapTexture.clear(sf::Color::White);
	shadowMapTexture.draw(g_shadowQuads);
	shadowMapTexture.display();

	compositeLightAndShadowTexture.clear(sf::Color::White);
	compositeLightAndShadowTexture.draw(shadowMapSprite, sf::BlendMultiply);
	compositeLightAndShadowTexture.draw(lightMapSprite, sf::BlendMultiply);
	compositeLightAndShadowTexture.display();

	lightShader.setUniform("lightMap", compositeLightAndShadowTexture.getTexture());

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

	/*_window.draw(tileSheet);*/
}

void Game::Cleanup()
{
	Engine::AssetsManager::Clear();
}
