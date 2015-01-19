#include "PathComponent.h"
#include "DebugMenu.h"

PathComponent::PathComponent(int checkType)
{
	type="Path";
	pType=checkType;
	isTurning=true;
	time=0;
	turn=true;
}

void PathComponent::Initialize(ObjectHandle* oh, int menuNum)
{
	parent=oh;

#if EDITOR_ON
	pathType=debugMenu.comboBox("Path Type", "Path", parent->menuNum);
	debugMenu.comboBoxItem("Begin-End", "Path", pathType, parent->menuNum);
	debugMenu.comboBoxItem("Back and Forth", "Path", pathType, parent->menuNum);
	debugMenu.comboBoxItem("Random", "Path", pathType, parent->menuNum);
	debugMenu.changeComboBox(pType, "Path", pathType, parent->menuNum);
	debugMenu.newRow("Path", parent->menuNum);
#endif
	path.parent=oh;
}

void PathComponent::InitializeLight(AmbientLight* lh, int menuNum)
{
	ambientParent=lh;

#if EDITOR_ON
	pathType=debugMenu.comboBox("Path Type", "Path", ambientParent->menuNum);
	debugMenu.comboBoxItem("Begin-End", "Path", pathType, ambientParent->menuNum);
	debugMenu.comboBoxItem("Back and Forth", "Path", pathType, ambientParent->menuNum);
	debugMenu.comboBoxItem("Random", "Path", pathType, ambientParent->menuNum);
	debugMenu.changeComboBox(pType, "Path", pathType, ambientParent->menuNum);
	debugMenu.newRow("Path", ambientParent->menuNum);
#endif
}

void PathComponent::InitializeDLight(DiffuseLight* lh, int menuNum)
{
	diffuseParent=lh;

#if EDITOR_ON
	pathType=debugMenu.comboBox("Path Type", "Path", diffuseParent->menuNum);
	debugMenu.comboBoxItem("Begin-End", "Path", pathType, diffuseParent->menuNum);
	debugMenu.comboBoxItem("Back and Forth", "Path", pathType, diffuseParent->menuNum);
	debugMenu.comboBoxItem("Random", "Path", pathType, diffuseParent->menuNum);
	debugMenu.changeComboBox(pType, "Path", pathType, diffuseParent->menuNum);
	debugMenu.newRow("Path", diffuseParent->menuNum);
#endif
}

void PathComponent::update(float dt)
{
	time+=dt;
#if EDITOR_ON
	pType=debugMenu.checkBoxItem("Path", pathType, parent->menuNum);
#endif

	if(path.isAtPosition(parent->realPosition))
	{
		parent->stopObject();
		if(!path.getNextPosition() && pType!=0)
		{
			path.createNewPath(parent->realPosition, pType);
			turn=true;
		}
		isTurning=true;
	}

	if(!isTurning)
	{
		parent->setXForce(path.updateMove(parent->realPosition, 100*dt, pType).x);
		parent->setYForce(path.updateMove(parent->realPosition, 100*dt, pType).y);
		parent->setZForce(path.updateMove(parent->realPosition, 100*dt, pType).z);
	}

	if(path.sizeOfPath()>0 && !parent->AIPriority && (turn || time>1))
	{
		glm::vec3 lookAt=parent->realPosition+glm::normalize(parent->realPosition-path.nextPosition());
		parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
		turn=false;
	}
	parent->AIPriority=false;
	isTurning=false;

	if(time>1)
		time=0;
}

void PathComponent::addNode(glm::vec3 nodePos)
{
	if(path.numNodes<20)
	{
		PathNode pn;
		pn.position=nodePos;
		path.addNode(pn);
		
		int check=path.numNodes-2;
		
		for(int i=0; i<=check; i++)
		{
			path.pathNodes[i].addConnection(&path.pathNodes[path.numNodes-1]);
			path.pathNodes[path.numNodes-1].addConnection(&path.pathNodes[i]);
		}
		
#if EDITOR_ON
		debugMenu.numberSpinBox(&path.pathNodes[path.numNodes-1].position.x, "X: ", "Path", parent->menuNum);
		debugMenu.numberSpinBox(&path.pathNodes[path.numNodes-1].position.y, "Y: ", "Path", parent->menuNum);
		debugMenu.numberSpinBox(&path.pathNodes[path.numNodes-1].position.z, "Z: ", "Path", parent->menuNum);
		debugMenu.newRow("Path", parent->menuNum);
#endif
	}
}