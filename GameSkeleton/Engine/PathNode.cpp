#include "PathNode.h"

void PathNode::addConnection(PathNode* connect)
{
	connections.push_back(connect);
}

void PathNode::removeConnection(PathNode* connect)
{
	for(int i=0; i<connections.size(); i++)
	{
		if(connections[i]==connect)
		{
			connections.erase(connections.begin()+i);
			return;
		}
	}
}