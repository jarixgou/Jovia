#ifndef ADD_NODE__HPP
#define ADD_NODE__HPP

#include "../Node.hpp"

class AddNode : public Node
{
	public:
	void CreateNodes(const int& _nodeId) override;
};

#endif