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
	/*DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
	{
		std::cout << "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << std::endl;
	}*/

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

	const int QUERY_BUFFER_SIZE = 8; // taille du buffer circulaire (à ajuster)
	GLuint queries[QUERY_BUFFER_SIZE] = { 0u };
	glGenQueries(QUERY_BUFFER_SIZE, queries);

	int writeIndex = 0;   // où on place la prochaine query
	int readIndex = 0;    // où on check la plus ancienne query non lue
	int outstanding = 0;  // nombre de queries en attente de résultat

	while (running)
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		Update(_window, time, dt);
		Engine::DebugInterface::Update(_window, dt, gpuTime);

		// Si buffer plein, tenter de libérer des résultats disponibles sans bloquer
		if (outstanding == QUERY_BUFFER_SIZE)
		{
			GLint available = 0;
			glGetQueryObjectiv(queries[readIndex], GL_QUERY_RESULT_AVAILABLE, &available);
			if (available)
			{
				GLuint64 elapsed = 0;
				glGetQueryObjectui64v(queries[readIndex], GL_QUERY_RESULT, &elapsed);
				gpuTime = static_cast<float>(elapsed) / 1000000.0f;
				readIndex = (readIndex + 1) % QUERY_BUFFER_SIZE;
				--outstanding;
			}
			else
			{
				// Pas de résultat prêt : on évite de bloquer, on rend sans mesurer cette frame
				Display(_window);
				continue;
			}
		}

		// Lancer la mesure pour cette frame sur la position writeIndex
		glBeginQuery(GL_TIME_ELAPSED, queries[writeIndex]);

		Display(_window);

		glEndQuery(GL_TIME_ELAPSED);

		// Marquer la query comme en attente puis avancer writeIndex
		writeIndex = (writeIndex + 1) % QUERY_BUFFER_SIZE;
		++outstanding;

		// Lire autant de résultats disponibles que possible (non bloquant)
		while (outstanding > 0)
		{
			GLint available = 0;
			glGetQueryObjectiv(queries[readIndex], GL_QUERY_RESULT_AVAILABLE, &available);
			if (!available) break;
			GLuint64 elapsed = 0;
			glGetQueryObjectui64v(queries[readIndex], GL_QUERY_RESULT, &elapsed);
			// elapsed en nanosecondes -> millisecondes
			gpuTime = static_cast<float>(elapsed) / 1000000.0f;
			readIndex = (readIndex + 1) % QUERY_BUFFER_SIZE;
			--outstanding;
		}
	}

	glDeleteQueries(QUERY_BUFFER_SIZE, queries);

	_window.setActive(false);
}