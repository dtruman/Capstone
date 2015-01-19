#pragma once
#include <GameWindowManager.h>
#include "Component.h"
#include "DebugMenu.h"
#include <time.h>
#include <locale>

class PlayerComponent : public Component
{
	ObjectHandle* parent;
	AmbientLight* ambientParent; //Need to edit script to allow light code
	DiffuseLight* diffuseParent;
public:
	PlayerComponent(glm::vec3 camPos=glm::vec3(0.0f,0.0f,0.0f), glm::vec3 camDir=glm::vec3(0.0f,0.0f,0.0f));
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	void Initialize(ObjectHandle*, int);
	void InitializeLight(AmbientLight*, int);
	void InitializeDLight(DiffuseLight*, int);
	void update(float dt);
};