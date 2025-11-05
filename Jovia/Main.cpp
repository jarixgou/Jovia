#include <thread>
#include <atomic>

#include "Common.h"
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
	while (running)
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		Update(_window, time, dt);
		Display(_window);
	}

	_window.setActive(false);
}