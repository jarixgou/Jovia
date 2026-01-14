#ifndef UV_NODE__HPP
#define UV_NODE__HPP

#include "../Node.hpp"

class UVNode : public Node
{
public:
	void CreateNodes(const int& _nodeId) override;
};

#endif