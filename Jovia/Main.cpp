#include <thread>
#include <atomic>

#include "Common.h"
#include "Engine/Interface/Debug/DebugInterface.hpp"
#include "Game/Cleanup/Cleanup.hpp"
#include "Game/Display/Display.hpp"
#include "Game/Init/Init.hpp"
#include "Game/PollEvents/PollEvents.hpp"
#include "Game/Update/Update.hpp"

WindowState windowState;

std::atomic<bool> running{ true };
std::atomic<bool> initialized{ false };

void GameLoopThread(sf::RenderWindow& _window);

int main()
{
	srand(time(NULL));

	std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>();

	Init(*window);

	initialized = true;

	window->setActive(false);

	std::thread renderThread(GameLoopThread, std::ref(*window));

	while (window->isOpen())
	{
		PollEvents(*window);
	}

	running = false;
	if (renderThread.joinable())
	{
		renderThread.join();
	}

	Cleanup();
}

void GameLoopThread(sf::RenderWindow& _window)
{
	while (!initialized)
	{
		sf::sleep(sf::milliseconds(10));
	}

	_window.setActive(true);

	sf::Clock deltaTime;
	float gpuTime = 0.0f;
	while (running)
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		Update(_window, time, dt);

		Engine::DebugInterface::Update(_window, dt, gpuTime);

		GLuint query;
		glGenQueries(1, &query);
		glBeginQuery(GL_TIME_ELAPSED, query);

		Display(_window);

		glEndQuery(GL_TIME_ELAPSED);

		GLuint64 elapsed;
		glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed);
		gpuTime = elapsed / 1000000.0f;

		glDeleteQueries(1, &query);
	}

	_window.setActive(false);
}