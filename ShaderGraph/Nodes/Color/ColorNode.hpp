#ifndef COLOR_NODE__HPP
#define COLOR_NODE__HPP

#include <SFML/Graphics/Color.hpp>

#include "../Node.hpp"

class ColorNode : public Node
{
public:
	sf::Color color;
	void CreateNodes(const int& _nodeId) override;
};

#endif