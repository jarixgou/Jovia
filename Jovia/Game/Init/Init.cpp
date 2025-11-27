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
    sf::ContextSettings windowSettings(32, 0, 32);
	_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia",sf::Style::Default, windowSettings);

    _window.setActive(true);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::string message = "GLEW initialization failed: " +
            std::string(reinterpret_cast<const char*>(glewGetErrorString(err)));
		LOG_CRITICAL(message.c_str(), true);
    }

    if (!GLEW_ARB_timer_query)
    {
        LOG_WARNING("Query timer not supported", true);
    }

    LOG_INFO("Render window was created successfully", true);

    std::string message = "OpenGL : " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    LOG_INFO(message.c_str(), true);

    _window.setActive(false);

	ImGui::SFML::Init(_window);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	LOG_INFO("ImGui was initialized", true);
}

void InitScene()
{
	Engine::ScenesManager::CreateScene<Menu>("Menu");
	Engine::ScenesManager::CreateScene<Game>("Game");

	Engine::ScenesManager::LoadScene("Game");
}