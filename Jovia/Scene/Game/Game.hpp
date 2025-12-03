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
	class GameObject;
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
	
	Engine::GameObject* monstre;

	sf::VertexArray colliderTest;
	sf::VertexArray colliderTest2;

	Engine::ChunkManager* m_chunkManager;
public:
	Game() = default;
	~Game() = default;

	void Init() override;
	void PollEvents(sf::Event& _event) override;
	void Update(float _dt) override;
	void Display() override;
	void Cleanup() override;
};
#endif