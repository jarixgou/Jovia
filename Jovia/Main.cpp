#include "Common.h"

// Prototypes
void Init();
void Update(sf::RenderWindow& _window, float);
void Display(sf::RenderWindow&);

sf::VertexArray vertexArray;

int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jovia");
	ImGui::SFML::Init(window);

	Init();

	sf::Vertex a(sf::Vector2f(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2));
	sf::Vertex b(sf::Vector2f(SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2));
	sf::Vertex c(sf::Vector2f(SCREEN_WIDTH / 2 + 32, SCREEN_HEIGHT / 2 + 32));
	sf::Vertex d(sf::Vector2f(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 + 32));

	vertexArray.setPrimitiveType(sf::PrimitiveType::LineStrip);

	vertexArray.append(a);
	vertexArray.append(b);
	vertexArray.append(c);
	vertexArray.append(d);
	vertexArray.append(a);

	sf::Clock deltaTime;
	while (window.isOpen())
	{
		sf::Time time = deltaTime.restart();
		float dt = time.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, time);

		Update(window, dt);
		Display(window);
	}
}



///////////////////////////////////////////////////////////////////
void Init()
{

}

void Update(sf::RenderWindow& _window, float _dt)
{
	sf::Vector2i mousePosPixel = sf::Mouse::getPosition(_window);
	sf::Vector2f mousePose = _window.mapPixelToCoords(mousePosPixel);

	int size = static_cast<int>(vertexArray.getVertexCount() - 1);
	for (int i = 0; i < size; ++i)
	{
		int a = i % size;
		int b = (i + 1) % size;

		sf::Vector2f AB = vertexArray[b].position - vertexArray[a].position;
		sf::Vector2f AM = mousePose - vertexArray[a].position;

		float dot = AB.x * AM.y - AB.y * AM.x;

		if (dot == 0.0f)
		{
			vertexArray[a].color = sf::Color::Red;
			vertexArray[b].color = sf::Color::Red;
			std::cout << a << b << std::endl;
		}
		else
		{
			vertexArray[a].color = sf::Color::White;
			vertexArray[b].color = sf::Color::White;
		}
	}
}

void Display(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	_window.draw(vertexArray);

	ImGui::SFML::Render(_window);
	_window.display();
}