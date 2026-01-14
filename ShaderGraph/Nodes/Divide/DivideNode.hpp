#ifndef DIVIDE_NODE__HPP
#define DIVIDE_NODE__HPP

#include "../Node.hpp"

class DivideNode : public Node
{
	public:
	void CreateNodes(const int& _nodeId) override;
};

#endif