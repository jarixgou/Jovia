#include "Init.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "../../Scene/Menu/Menu.hpp"
#include "../../Scene/Game/Game.hpp"

void InitSystem(sf::RenderWindow& _window);
void InitScene();

void Init(sf::RenderWindow& _window)
{
	InitSystem(_window);
	InitScene();
}

void InitSystem(sf::RenderWindow& _window)
{
	_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia");

    _window.setActive(true);

    // INITIALISER GLEW ICI
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::string message = std::string("GLEW initialization failed: ") +
            reinterpret_cast<const char*>(glewGetErrorString(err));
		LOG_CRITICAL(message.c_str());
    }

    // Vérifier que les extensions nécessaires sont disponibles
    if (!GLEW_ARB_timer_query)
    {
        LOG_WARNING("Query timer not supported");
    }

    _window.setActive(false);

    LOG_DEBUG("Render window was created successfully");

	ImGui::SFML::Init(_window);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	LOG_DEBUG("ImGui was initialized");
}

void InitScene()
{
	Engine::ScenesManager::CreateScene<Menu>("Menu");
	Engine::ScenesManager::CreateScene<Game>("Game");

	Engine::ScenesManager::LoadScene("Game");
}