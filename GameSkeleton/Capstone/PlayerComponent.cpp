#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(glm::vec3 camPos, glm::vec3 camDir)
{
	cameraPosition=camPos;
	cameraDirection=camDir;
}

void PlayerComponent::Initialize(ObjectHandle* oh, int menuNum)
{
	parent=oh;

#if EDITOR_ON
	debugMenu.numberSpinBox(&cameraPosition.x, "X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.y, "Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.z, "Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.x, "Direction X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.y, "Direction Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.z, "Direction Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
#endif
	type="Player";
}

void PlayerComponent::InitializeLight(AmbientLight* lh, int menuNum)
{
	ambientParent=lh;

#if EDITOR_ON
	debugMenu.numberSpinBox(&cameraPosition.x, "X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.y, "Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.z, "Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.x, "Direction X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.y, "Direction Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.z, "Direction Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
#endif
	type="Player";
}

void PlayerComponent::InitializeDLight(DiffuseLight* lh, int menuNum)
{
	diffuseParent=lh;

#if EDITOR_ON
	debugMenu.numberSpinBox(&cameraPosition.x, "X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.y, "Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraPosition.z, "Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.x, "Direction X: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.y, "Direction Y: ", "Player Camera", menuNum);
	debugMenu.numberSpinBox(&cameraDirection.z, "Direction Z: ", "Player Camera", menuNum);
	debugMenu.newRow("Player Camera", menuNum);
#endif
	type="Player";
}

void PlayerComponent::update(float dt)
{
}