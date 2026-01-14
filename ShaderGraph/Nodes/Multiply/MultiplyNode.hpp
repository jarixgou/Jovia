#ifndef MULTIPLY_NODE__HPP
#define MULTIPLY_NODE__HPP

#include "../Node.hpp"

class MultiplyNode : public Node
{
	public:
	void CreateNodes(const int& _nodeId) override;
};

#endif