#ifndef TEXTURE_NODE__HPP
#define TEXTURE_NODE__HPP
#include "../Node.hpp"

class TextureNode : public Node
{
public:
	void CreateNodes(const int& _nodeId) override;
};

#endif