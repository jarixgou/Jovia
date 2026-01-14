#ifndef SHADER_GRAPH__HPP
#define SHADER_GRAPH__HPP
#include <vector>

class Node;
struct Link;

struct ShaderGraph
{
	int idCount;
	std::vector<Node*> nodes;
};

#endif