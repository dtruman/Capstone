#include "GameWindowManager.h"

GameWindowManager::GameWindowManager()
{
	numVariables=0;
	hitOn=false;
	currentObj="";
}

bool GameWindowManager::detectContacts(std::string myObject, std::string otherObject)
{
	if(runWorld)
	{
		for(int k=0; k<CS.size(); k++)
		{
			if((myObject.compare(CS[k].nameOne)==0 || myObject.compare(CS[k].nameTwo)==0) && (otherObject.compare(CS[k].nameOne)==0 || otherObject.compare(CS[k].nameTwo)))
				return true;
		}
		return false;
	}
	return false;
}

bool GameWindowManager::afterAnyContact(std::string myObject)
{
	hitOff=true;
	for(int k=0; k<CS.size(); k++)
	{
		if(!hitOn && (myObject.compare(CS[k].nameOne)==0 || myObject.compare(CS[k].nameTwo)==0))
		{
			hitOn=true;
			currentObj=myObject;
		}

		if(hitOn && hitOff)
		{
			if((myObject.compare(CS[k].nameOne)==0 || myObject.compare(CS[k].nameTwo)==0))
  				hitOff=false;
		}
	}
	if(hitOff)
		hitOff=hitOn;
	if(myObject.compare(currentObj)!=0)
		hitOff=false;
	if(hitOff)
		hitOn=false;
	return hitOff;
}

bool GameWindowManager::detectAnyContact(std::string myObject)
{
	for(int k=0; k<CS.size(); k++)
	{
		if(myObject.compare(CS[k].nameOne)==0 || myObject.compare(CS[k].nameTwo)==0)
			return true;
	}
	return false;
}

bool GameWindowManager::detectContactsContaining(std::string myObject, std::string containing)
{
	for(int k=0; k<CS.size(); k++)
	{
		if((CS[k].nameOne.compare(myObject)==0 || CS[k].nameTwo.compare(myObject)==0) && (CS[k].nameOne.find(containing)!=std::string::npos || CS[k].nameTwo.find(containing)!=std::string::npos))
			return true;
	}
	return false;
}

void GameWindowManager::globalVariableHandle(std::string passedIn)
{
	std::string newVariable="";
	int track=0;

	if(passedIn.find("=")==std::string::npos)
	{
		return;
	}

	while(passedIn.at(track)!=';')
	{
		newVariable+=passedIn.at(track++);
	}

	bool alreadyAVariable=false;
	int which=-1;
	for(int i=0; i<numVariables; i++)
	{
		if(globalVariables[i].variableName.compare(newVariable.substr(0, newVariable.find("=")))==0)
		{
			alreadyAVariable=true;
			which=i;
		}
	}

	if(!alreadyAVariable)
	{
		globalVariables[numVariables].variableName=newVariable.substr(0, newVariable.find("="));
		globalVariables[numVariables++].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
	else
	{
		//globalVariables[which].value=newVariable.substr(newVariable.find("=")+1, newVariable.length());
	}
}