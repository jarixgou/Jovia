#include "Init.hpp"

#include <Engine/Scene/ScenesManager.hpp>

#include "../../Scene/Menu/Menu.hpp"
#include "../../Scene/Game/Game.hpp"
#include "Engine/System/System.hpp"

void InitSystem();
void InitScene();

void Init()
{
	InitSystem();
	InitScene();
}

void InitSystem()
{
    System::window = std::make_unique<sf::RenderWindow>();
    System::window->create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia",sf::Style::Default);

    System::window->setActive(true);

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

    System::window->setActive(false);

	ImGui::SFML::Init(*System::window);
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