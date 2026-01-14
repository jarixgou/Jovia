#ifndef SUBTRACT_NODE__HPP
#define SUBTRACT_NODE__HPP

#include "../Node.hpp"

class SubtractNode : public Node
{
	public:
	void CreateNodes(const int& _nodeId) override;
};

#endif