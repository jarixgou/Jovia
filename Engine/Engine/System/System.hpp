#ifndef SYSTEM__HPP
#define SYSTEM__HPP
#include <SFML/Graphics/RenderWindow.hpp>

struct System
{
	static std::unique_ptr<sf::RenderWindow> window;
	static bool drawLight;
	static int verticeNb;
	static int drawCall;
};

#endif