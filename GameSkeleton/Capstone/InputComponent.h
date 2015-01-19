#pragma once
#include "Component.h"
#include "ScriptComponent.h"
#include <time.h>
#include <locale>

class InputComponent : public Component
{
	ObjectHandle* parent;
	AmbientLight* ambientParent; //Need to edit script to allow light code
	DiffuseLight* diffuseParent;
	float time;
	std::string tabName;
public:
	InputComponent(std::string nameOfTab, int upNum=0,int actionNum=0, std::string actionScript="");
	int keyCode;
	int defaultCode;
	int updateNum;
	int actionNum;
	ScriptComponent* sc;
	void Initialize(ObjectHandle*, int);
	void InitializeLight(AmbientLight*, int);
	void InitializeDLight(DiffuseLight*, int);
	void update(float dt);
	void handleAction(float dt);
};