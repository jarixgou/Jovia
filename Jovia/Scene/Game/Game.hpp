#ifndef GAME__HPP
#define GAME__HPP

#include <Engine/Scene/Scene.hpp>

#include "../../Common.h"
#include "Engine/TextureSlice/TextureSlice.hpp"

namespace Engine
{
	class Camera;
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
	std::vector<Engine::Light*> lightList;

	sf::VertexArray g_shadowQuads;

	sf::RenderTexture lightMapTexture;
	sf::Sprite lightMapSprite;

	sf::RenderTexture fuseComposite;
	sf::Sprite compositeLightAndShadowSprite;

	sf::RenderTexture sceneTexture;
	sf::Sprite sceneSprite;

	sf::RenderTexture m_compositeShadow;
	sf::Sprite m_compositeShadowSprite;

	sf::VertexArray colliderTest;
	sf::VertexArray colliderTest2;

	sf::RenderStates lightShaderStates;
	sf::Shader lightShader;

	sf::CircleShape lightPosCircle;

	Engine::ChunkManager* m_chunkManager;
public:
	Game() = default;
	~Game() = default;

	void Init() override;
	void PollEvents(sf::RenderWindow& _window, sf::Event& _event) override;
	void Update(sf::RenderWindow& _renderWindow, float _dt) override;
	void Display(sf::RenderWindow& _window) override;
	void Cleanup() override;
};
#endif