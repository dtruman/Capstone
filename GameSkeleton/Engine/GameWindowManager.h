#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"
#include <iostream>
#include <vector>
#include "ExportHeader.h"

class ENGINE_SHARED GameWindowManager
{
	bool hitOn;
	bool hitOff;
	std::string currentObj;
public:
	//In MyGLWindow, update contactSource list by running through all the contacts and applying the correct names that correspond to each contact instance. then you can do as normal like you would in the functions
	//Provided in this class
	struct contactSources
	{
		std::string nameOne;
		std::string nameTwo;
	};
	struct variables{
		std::string variableName;
		std::string value;
	} globalVariables[100];

	int numVariables;

	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<contactSources> CS;
	bool* runWorld;
	GameWindowManager();
	bool afterAnyContact(std::string myObject);
	bool detectContacts(std::string myObject, std::string otherObject);
	bool detectAnyContact(std::string myObject);
	bool detectContactsContaining(std::string myObject, std::string containing);
	void globalVariableHandle(std::string passedIn);
};