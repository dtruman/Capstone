#pragma once
#include <GameWindowManager.h>
#include "Component.h"
#include <locale>

class ScriptComponent : public Component
{
	struct variables{
		std::string variableName;
		std::string value;
	} myVariables[100];

	int numVariables;

	ObjectHandle* parent;
	AmbientLight* ambientParent; //Need to edit script to allow light code
	DiffuseLight* diffusParent;
	bool isNumber(const std::string& s);
	bool insensitiveStringComparision(std::string s, std::string d);
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	std::string getMiddleString(char first, char last, std::string checkString);
	float handleNumbers(std::string parse);
	float handleEquations(char sign, std::string firstParam, std::string secondParam);
	void handleSetX(std::string);
	void handleSetY(std::string);
	void handleSetZ(std::string);
	void handleSetAll(std::string);
	void handleSetScaleX(std::string);
	void handleSetScaleY(std::string);
	void handleSetScaleZ(std::string);
	void handleSetScaleAll(std::string);
	void handleSetVelocityX(std::string);
	void handleSetVelocityY(std::string);
	void handleSetVelocityZ(std::string);
	void handleSetVelocityAll(std::string);
	void handleSetRotationX(std::string);
	void handleSetRotationY(std::string);
	void handleSetRotationZ(std::string);
	void handleSetRotationAll(std::string);
	void handleSetForceX(std::string);
	void handleSetForceY(std::string);
	void handleSetForceZ(std::string);
	void handleSetForceAll(std::string);
	void handleMaintainForce(std::string);
	void handleMaintainVelocity(std::string);
	void handleMaintainXVelocity(std::string);
	void handleMaintainZVelocity(std::string);
	void handleSetLvl(std::string);
	void handleVariableDeclaration(std::string);
	bool handleTrueCheck(std::string);
	bool handleEquals(std::string);
	bool handleNotEquals(std::string);
	void handleGrabObject(std::string);
	void scriptUpdate(std::string script);
public:
	bool firstRun;
	ScriptComponent(std::string passScript="");
	std::string script;
	void Initialize(ObjectHandle*, int);
	void InitializeLight(AmbientLight*, int);
	void InitializeDLight(DiffuseLight*, int);
	void InitializeInput(ObjectHandle*);
	void update(float dt);
};