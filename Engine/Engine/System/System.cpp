#include "System.hpp"

std::unique_ptr<sf::RenderWindow> System::window = nullptr;
bool System::drawLight = false;
int System::drawCall = 0;
int System::verticeNb = 0;