#ifndef GAME__HPP
#define GAME__HPP

#include <Engine/Scene/Scene.hpp>

#include "../../Common.h"
#include "Engine/TextureSlice/TextureSlice.hpp"

namespace Engine
{
	class Camera;
	class DrawableObject;
	class ChunkManager;
	class Light;
}

class Game : public Engine::Scene
{
private:
	Engine::Camera* m_camera;

	sf::Sprite m_tileSheet;

	std::vector<Engine::TextureSliced> m_textureSliced;

	sf::RenderStates m_renderStates;

	sf::CircleShape lightCircle;
	sf::CircleShape lightCircle2;
	Engine::Light* light;

	sf::VertexArray g_shadowQuads;

	sf::RenderTexture shadowMapTexture;
	sf::Sprite shadowMapSprite;

	sf::RenderTexture lightMapTexture;
	sf::Sprite lightMapSprite;

	sf::RenderTexture compositeLightAndShadowTexture;
	sf::Sprite compositeLightAndShadowSprite;

	sf::RenderTexture sceneTexture;
	sf::Sprite sceneSprite;

	sf::VertexArray colliderTest;
	sf::VertexArray colliderTest2;

	sf::RenderStates lightShaderStates;
	sf::Shader lightShader;

	Engine::ChunkManager* m_chunkManager;
public:
	Game() = default;
	virtual ~Game() = default;

	virtual void Init() override;
	virtual void PollEvents(sf::RenderWindow& _window, sf::Event& _event) override;
	virtual void Update(sf::RenderWindow& _renderWindow, float _dt) override;
	virtual void Display(sf::RenderWindow& _window) override;
	virtual void Cleanup() override;
};
#endif