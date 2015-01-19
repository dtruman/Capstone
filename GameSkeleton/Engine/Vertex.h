#pragma once

#include "ExportHeader.h"
#include <glm.hpp>

struct ENGINE_SHARED DLTVertex
{
public:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 uv;
	static unsigned int POSITION_OFFSET;
	static unsigned int COLOR_OFFSET;
	static unsigned int NORMAL_OFFSET;
	static unsigned int UV_OFFSET;
	static unsigned int STRIDE;
};