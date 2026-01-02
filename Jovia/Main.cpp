#include <thread>
#include <atomic>

#include "Common.h"
#include "Engine/Interface/Debug/DebugInterface.hpp"
#include "Engine/System/System.hpp"
#include "Game/Cleanup/Cleanup.hpp"
#include "Game/Display/Display.hpp"
#include "Game/Init/Init.hpp"
#include "Game/PollEvents/PollEvents.hpp"
#include "Game/Update/Update.hpp"

WindowState windowState;

void GameLoopThread(std::stop_token _stopToken);

int main()
{
	/*DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
	{
		std::cout << "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << std::endl;
	}*/

	srand(time(NULL));

	Init();

	Engine::System::window->setActive(false);

	std::jthread gameLoop(GameLoopThread);

	while (Engine::System::window->isOpen())
	{
		PollEvents();
	}

	gameLoop.request_stop();

	Cleanup();
}

void GameLoopThread(std::stop_token _stopToken)
{
	Engine::System::window->setActive(true);

	float gpuTime = 0.0f;

	const int queryBufferSize = 8; // taille du buffer circulaire (à ajuster)
	GLuint queries[queryBufferSize] = { 0 };
	glGenQueries(queryBufferSize, queries);

	int writeIndex = 0;   // où on place la prochaine query
	int readIndex = 0;    // où on check la plus ancienne query non lue
	int outstanding = 0;  // nombre de queries en attente de résultat

	while (!_stopToken.stop_requested())
	{
		Engine::System::time.Restart();

		Update();
		Engine::DebugInterface::Update(gpuTime);

		// Si buffer plein, tenter de libérer des résultats disponibles sans bloquer
		if (outstanding == queryBufferSize)
		{
			GLint available = 0;
			glGetQueryObjectiv(queries[readIndex], GL_QUERY_RESULT_AVAILABLE, &available);
			if (available)
			{
				GLuint64 elapsed = 0;
				glGetQueryObjectui64v(queries[readIndex], GL_QUERY_RESULT, &elapsed);
				gpuTime = static_cast<float>(elapsed) / 1000000.0f;
				readIndex = (readIndex + 1) % queryBufferSize;
				--outstanding;
			}
			else
			{
				// Pas de résultat prêt : on évite de bloquer, on rend sans mesurer cette frame
				Display();
				continue;
			}
		}

		// Lancer la mesure pour cette frame sur la position writeIndex
		glBeginQuery(GL_TIME_ELAPSED, queries[writeIndex]);

		Display();

		glEndQuery(GL_TIME_ELAPSED);

		// Marquer la query comme en attente puis avancer writeIndex
		writeIndex = (writeIndex + 1) % queryBufferSize;
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
			readIndex = (readIndex + 1) % queryBufferSize;
			--outstanding;
		}
	}

	glDeleteQueries(queryBufferSize, queries);

	Engine::System::window->setActive(false);
}