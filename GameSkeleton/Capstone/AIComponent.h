#pragma once
#include <Component.h>
#include "ScriptComponent.h"

class AIComponent : public Component
{
	ObjectHandle* parent;
	AmbientLight* ambientParent;
	DiffuseLight* diffuseParent;
	int AIType;
	ObjectHandle* playerTrack;
	bool playerSet;
	float timeStamp;
	float stayAwayRange;
	float goTowardsRange;
	glm::vec3 guidePosition;
public:
	int AIT;
	ScriptComponent* sc;
	AIComponent(std::string script="", int t=-1);
	void Initialize(ObjectHandle*, int);
	void InitializeLight(AmbientLight*, int);
	void InitializeDLight(DiffuseLight*, int);
	void update(float dt);
	void passPlayer(ObjectHandle* player);
};