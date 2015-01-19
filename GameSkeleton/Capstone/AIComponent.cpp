#include "AIComponent.h"

AIComponent::AIComponent(std::string script, int t)
{
	type="AI";
	sc=new ScriptComponent(script);
	AIT=t;
	playerSet=false;
	timeStamp=0;
	guidePosition=glm::vec3(0.0f,0.0f,0.0f);
	stayAwayRange=0.0f;
	goTowardsRange=0.0f;
}

void AIComponent::Initialize(ObjectHandle* oh, int menuNum)
{
	parent=oh;
	sc->InitializeInput(oh);

#if EDITOR_ON
	AIType=debugMenu.comboBox("KeyCode", "AI", parent->menuNum);
	debugMenu.comboBoxItem("None", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards and Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.changeComboBox(AIT, "AI", AIType, parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&stayAwayRange, "Run From Range: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);
	debugMenu.numberSpinBox(&goTowardsRange, "Approach  Range: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&guidePosition.x, "X: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.y, "Y: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.z, "Z: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", "AI", parent->menuNum);
#endif
}

void AIComponent::InitializeLight(AmbientLight* oh, int menuNum)
{
	ambientParent=oh;

#if EDITOR_ON
	AIType=debugMenu.comboBox("KeyCode", "AI", parent->menuNum);
	debugMenu.comboBoxItem("None", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards and Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.changeComboBox(AIT, "AI", AIType, parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&stayAwayRange, "Run Range: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&goTowardsRange, "Stalk Range: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&guidePosition.x, "X: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.y, "Y: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.z, "Z: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", "AI", parent->menuNum);
#endif
}

void AIComponent::InitializeDLight(DiffuseLight* oh, int menuNum)
{
	diffuseParent=oh;

#if EDITOR_ON
	AIType=debugMenu.comboBox("KeyCode", "AI", parent->menuNum);
	debugMenu.comboBoxItem("None", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Run Away from Place", "AI", AIType, parent->menuNum);
	debugMenu.comboBoxItem("Go Towards and Away from Player", "AI", AIType, parent->menuNum);
	debugMenu.changeComboBox(AIT, "AI", AIType, parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&stayAwayRange, "Run Range: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&goTowardsRange, "Stalk Range: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.numberSpinBox(&guidePosition.x, "X: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.y, "Y: ", "AI", parent->menuNum);
	debugMenu.numberSpinBox(&guidePosition.z, "Z: ", "AI", parent->menuNum);
	debugMenu.newRow("AI", parent->menuNum);

	debugMenu.basicTextField(&sc->script, "Action", "AI", parent->menuNum);
#endif
}

void AIComponent::update(float dt)
{
	if(parent)
	{
#if EDITOR_ON
		AIT=debugMenu.checkBoxItem("AI", AIType, parent->menuNum);
#endif
	}
	timeStamp+=dt;
	if(AIT==1 && playerSet)
	{
		if(glm::length(playerTrack->realPosition-parent->realPosition)<goTowardsRange)
		{
			parent->AIPriority=true;
			parent->setXForce(glm::normalize(playerTrack->realPosition-parent->realPosition).x*(100*dt));
			parent->setYForce(glm::normalize(playerTrack->realPosition-parent->realPosition).y*(100*dt));
			parent->setZForce(glm::normalize(playerTrack->realPosition-parent->realPosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(parent->realPosition-parent->realPosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
	}
	else if(AIT==2 && playerTrack)
	{
		if(glm::length(playerTrack->realPosition-parent->realPosition)<stayAwayRange)
		{
			parent->AIPriority=true;
			parent->setXForce(glm::normalize(parent->realPosition-playerTrack->realPosition).x*(100*dt));
			parent->setYForce(glm::normalize(parent->realPosition-playerTrack->realPosition).y*(100*dt));
			parent->setZForce(glm::normalize(parent->realPosition-playerTrack->realPosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(parent->realPosition-parent->realPosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
	}
	else if(AIT==3 && playerTrack)
	{
		if(glm::length(guidePosition-parent->realPosition)<goTowardsRange)
		{
			parent->AIPriority=true;
			parent->setXForce(glm::normalize(guidePosition-parent->realPosition).x*(100*dt));
			parent->setYForce(glm::normalize(guidePosition-parent->realPosition).y*(100*dt));
			parent->setZForce(glm::normalize(guidePosition-parent->realPosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(parent->realPosition-guidePosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
	}
	else if(AIT==4 && playerTrack)
	{
		if(glm::length(guidePosition-parent->realPosition)<stayAwayRange)
		{
			parent->AIPriority=true;
			parent->setXForce(glm::normalize(parent->realPosition-guidePosition).x*(100*dt));
			parent->setYForce(glm::normalize(parent->realPosition-guidePosition).y*(100*dt));
			parent->setZForce(glm::normalize(parent->realPosition-guidePosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(guidePosition-parent->realPosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
	}
	else if(AIT==5 && playerTrack)
	{
		if(glm::length(playerTrack->realPosition-parent->realPosition)<goTowardsRange)
		{
			parent->AIPriority=true;
			parent->setXForce(glm::normalize(playerTrack->realPosition-parent->realPosition).x*(100*dt));
			parent->setYForce(glm::normalize(playerTrack->realPosition-parent->realPosition).y*(100*dt));
			parent->setZForce(glm::normalize(playerTrack->realPosition-parent->realPosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(parent->realPosition-playerTrack->realPosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
		if(glm::length(playerTrack->realPosition-parent->realPosition)<stayAwayRange)
		{
			parent->setXForce(glm::normalize(parent->realPosition-playerTrack->realPosition).x*(100*dt));
			parent->setYForce(glm::normalize(parent->realPosition-playerTrack->realPosition).y*(100*dt));
			parent->setZForce(glm::normalize(parent->realPosition-playerTrack->realPosition).z*(100*dt));

			if(timeStamp>1)
			{
				glm::vec3 lookAt=parent->realPosition+glm::normalize(playerTrack->realPosition-parent->realPosition);
				parent->setYaw(std::atan2((lookAt.x-parent->realPosition.x), (lookAt.z-parent->realPosition.z)));
				timeStamp=0;
			}
		}
	}
}

void AIComponent::passPlayer(ObjectHandle* player)
{
	playerTrack=player;
	playerSet=true;
}