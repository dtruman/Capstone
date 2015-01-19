#include "Path.h"

Path::Path()
{
	numNodes=0;
}

bool Path::isAtPosition(glm::vec3 current)
{
	if(currentPath.size()>0)
	{
		if(glm::length(currentPath[0].position-current)<2.0f)
		{
			return true;
		}
	}
	return false;
}

bool Path::getNextPosition()
{
	if(currentPath.size()>1)
	{
		currentPath.erase(currentPath.begin()+0);
		return true;
	}
	currentPath.erase(currentPath.begin()+0);
	return false;
}

int Path::sizeOfPath()
{
	return currentPath.size();
}

glm::vec3 Path::nextPosition()
{
	if(currentPath.size()>0)
		return currentPath[0].position;
	else
		return glm::vec3(0.0f,0.0f,0.0f);
}

glm::vec3 Path::updateMove(glm::vec3 current, float speed, int type)
{
	if(currentPath.size()>0)
	{
		glm::vec3 next=currentPath[0].position-current;
		next=glm::normalize(next);

		return (next*speed);
	}
	else
	{
		createNewPath(current, type);
		return glm::vec3(0.0f,0.0f,0.0f);
	}
}

void Path::createNewPath(glm::vec3 current, int type)
{
	bool destinationSet=false;
	PathNode nextDestination;

	//Figure out pathing stuff here please (Note To Devon Truman, aka myself)

	if(type==0)
	{
 		if(glm::length(current-pathNodes[numNodes-1].position)<2.0f)
		{
			type=0;
		}
		else
		{
			for(int i=0; i<numNodes; i++)
			{
				if(currentPath.size()==0 && !parent->raycast(pathNodes[i].position, parent->realPosition))
					currentPath.push_back(pathNodes[i]);
				else if(currentPath.size()>0 && !parent->raycast(pathNodes[i].position, currentPath[currentPath.size()-1].position))
					currentPath.push_back(pathNodes[i]);
			}
		}
	}
	else if(type==1)
	{
		if(glm::length(current-pathNodes[0].position)<2.0f)
		{
			for(int i=1; i<numNodes; i++)
			{
				if(!parent->raycast(pathNodes[i].position, parent->realPosition))
					currentPath.push_back(pathNodes[i]);
			}
		}
		else if(glm::length(current-pathNodes[numNodes-1].position)<2.0f)
		{
			for(int i=numNodes-2; i>=0; i--)
			{
				if(!parent->raycast(pathNodes[i].position, parent->realPosition))
					currentPath.push_back(pathNodes[i]);
			}
		}
		else
		{
			for(int i=0; i<numNodes; i++)
			{
				if(currentPath.size()==0 && !parent->raycast(pathNodes[i].position, parent->realPosition))
					currentPath.push_back(pathNodes[i]);
				else if(currentPath.size()>0 && !parent->raycast(pathNodes[i].position, currentPath[currentPath.size()-1].position))
					currentPath.push_back(pathNodes[i]);
			}
		}
	}
	else if(type==2)
	{
		int destination=rand()%numNodes;

		nextDestination=pathNodes[destination];
		destinationSet=true;
	}

	//Do all the math and stuff here
	if(!destinationSet)
	{
		return;
	}

	if(parent->raycast(nextDestination.position, parent->realPosition))
		currentPath.push_back(nextDestination);
}

void Path::addNode(PathNode pn)
{
	pathNodes[numNodes++]=pn;
}