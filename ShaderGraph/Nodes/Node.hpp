#ifndef NODE__HPP
#define NODE__HPP
#include <vector>

struct Pin;

enum class NodeType
{
	TEXTURE,
	UV,
	COLOR,
	ADD,
	MULTIPLY,
	SUBTRACT,
	DIVIDE,

};

class Node
{
public:
	int id;
	NodeType type;

	std::vector<Pin> inputPins;
	std::vector<Pin> outputPins;

	virtual void CreateNodes(const int& _nodeId);
};

#endif