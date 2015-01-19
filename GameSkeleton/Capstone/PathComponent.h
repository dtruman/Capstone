#pragma once
#include <GameWindowManager.h>
#include "Path.h"
#include "Component.h"

class PathComponent : public Component
{
	ObjectHandle* parent;
	AmbientLight* ambientParent; //Need to edit script to allow light code
	DiffuseLight* diffuseParent;
	float time;
	std::string tabName;
	bool isTurning;
	int pathType;
	bool turn;
public:
	int pType;
	PathComponent(int checkType=-1);
	void Initialize(ObjectHandle*, int);
	void InitializeLight(AmbientLight*, int);
	void InitializeDLight(DiffuseLight*, int);
	void update(float dt);
	void addNode(glm::vec3 nodePos=glm::vec3(0.0f,0.0f,0.0f));
	Path path;
};