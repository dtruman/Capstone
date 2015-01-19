#pragma once

#include <glm.hpp>
#include <vector>
#include "Engine.h"

class ENGINE_SHARED PathNode
{
public:
	glm::vec3 position;
	std::vector<PathNode*> connections;
	void addConnection(PathNode*);
	void removeConnection(PathNode*);
};