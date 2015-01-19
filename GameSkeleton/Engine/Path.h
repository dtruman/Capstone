#pragma once

#include "PathNode.h"
#include <glm.hpp>
#include <vector>
#include <random>
#include "Engine.h"
#include "ObjectHandle.h"

class ENGINE_SHARED Path
{
	std::vector<PathNode> currentPath;
public:
	ObjectHandle* parent;
	Path();
	PathNode pathNodes[20];
	bool isAtPosition(glm::vec3);
	bool getNextPosition();
	glm::vec3 nextPosition();
	glm::vec3 updateMove(glm::vec3 current, float speed, int type=2);
	int sizeOfPath();
	void createNewPath(glm::vec3, int type=2);
	void addNode(PathNode);
	int numNodes;
};