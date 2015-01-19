#include "Vertex.h"

unsigned int DLTVertex::POSITION_OFFSET=0;
unsigned int DLTVertex::COLOR_OFFSET=sizeof(glm::vec3);
unsigned int DLTVertex::NORMAL_OFFSET=sizeof(glm::vec3)+sizeof(glm::vec4);
unsigned int DLTVertex::UV_OFFSET=(sizeof(glm::vec3)*2)+sizeof(glm::vec4);
unsigned int DLTVertex::STRIDE=(sizeof(glm::vec3)*2)+sizeof(glm::vec4)+sizeof(glm::vec2);