#include "Common.h"
#include <cmath> // pour std::abs

#include "Palette/Palette.h"
#include "Debug/Debug.h"

// Prototypes
void Init();
void Update(sf::RenderWindow& _window, float);
void Display(sf::RenderWindow&);

sf::VertexArray vertexArray;

WindowState windowState;

int main()
{
	srand(time(NULL));

	std::unique_ptr<sf::RenderWindow> window = std::make_unique<sf::RenderWindow>(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia");
	ImGui::SFML::Init(*window);

	Init();

	sf::Vertex a(sf::Vector2f(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2));
	sf::Vertex b(sf::Vector2f(SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2));
	sf::Vertex c(sf::Vector2f(SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2 + 32));
	sf::Vertex d(sf::Vector2f(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 32));

	vertexArray.setPrimitiveType(sf::PrimitiveType::Lines);

	vertexArray.append(a);
	vertexArray.append(b);

	vertexArray.append(b);
	vertexArray.append(c);

	vertexArray.append(c);
	vertexArray.append(d);

	vertexArray.append(d);
	vertexArray.append(a);

	sf::Clock deltaTime;
	while (window->isOpen())
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		sf::Event event;
		while (window->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(*window, event);
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window->close();
			}
		}

		ImGui::SFML::Update(*window, time);

		Update(*window, dt);
		Display(*window);

		if (windowState.requestFullscreen)
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

			ImGui::SFML::Init(*window); // Réinitialise ImGui-SFML
			windowState.requestFullscreen = false;
		}
	}
}



///////////////////////////////////////////////////////////////////
void Init()
{

}

void Update(sf::RenderWindow& _window, float _dt)
{
	Palette::Update(_window, _dt);

	sf::Vector2i mousePosPixel = sf::Mouse::getPosition(_window);
	sf::Vector2f mousePose = _window.mapPixelToCoords(mousePosPixel);

	const float epsilon = 1e-2f;

	for (size_t i = 0; i < vertexArray.getVertexCount(); ++i)
	{
		vertexArray[i].color = sf::Color::White;
	}

	for (size_t i = 0; i < vertexArray.getVertexCount(); i += 2)
	{
		sf::Vector2f A = vertexArray[i].position;
		sf::Vector2f B = vertexArray[i + 1].position;
		sf::Vector2f AB = B - A;
		sf::Vector2f AM = mousePose - A;

		float cross = AB.x * AM.y - AB.y * AM.x;
		if (std::abs(cross) < epsilon)
		{
			float dot = AM.x * AB.x + AM.y * AB.y;
			float lenSq = AB.x * AB.x + AB.y * AB.y;
			if (dot >= 0 && dot <= lenSq)
			{
				vertexArray[i].color = sf::Color::Red;
				vertexArray[i + 1].color = sf::Color::Red;
			}
		}
	}

	Debug::Update(_dt, windowState, _window);
}

void Display(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	_window.draw(vertexArray);

	ImGui::SFML::Render(_window);
	_window.display();
}