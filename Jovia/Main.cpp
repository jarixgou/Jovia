#include "Common.h"
#include "Game/Cleanup/Cleanup.hpp"
#include "Game/Display/Display.hpp"
#include "Game/Init/Init.hpp"
#include "Game/PollEvents/PollEvents.hpp"
#include "Game/Update/Update.hpp"


WindowState windowState;

int main()
{
	srand(time(NULL));

	std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>();

	Init(*window);

	sf::Clock deltaTime;
	while (window->isOpen())
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		PollEvents(*window);

		Update(*window, time, dt);

		Display(*window);

		/*if (windowState.requestFullscreen)
		{
			window->close();
			if (windowState.fullscreen)
			{
				RECT rect;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
				window->create(sf::VideoMode(rect.right, rect.bottom), "Jovia", sf::Style::Fullscreen);
			}
			else
			{ 
				window->create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia", sf::Style::Default);
			}

			ImGui::SFML::Init(*window);
			windowState.requestFullscreen = false;
		}*/
	}

	Cleanup();
}