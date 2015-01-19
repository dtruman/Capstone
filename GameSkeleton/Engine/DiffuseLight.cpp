#include "DiffuseLight.h"
#include "Component.h"

DiffuseLight::DiffuseLight(std::string ID, glm::vec3 pos, glm::vec3 col, float siz)
{
	QWidget* tempLay=new QWidget();

	nameID=ID;
	tempLay->setFixedWidth(300);
	tempLay->setMinimumHeight(600);
	color=col;
	position=pos;
	size=siz;
	deleteLight=false;
	cloneLight=false;

	int replace=1;
	std::string tempName=nameID;
#if EDITOR_ON
	while(debugMenu.alreadyInWorld(nameID))
	{
		nameID=tempName+std::to_string(replace);
		replace++;
	}

	debugMenu.listItem(&nameID, "World Control");
	menuOn=true;
	debugMenu.addLayout("Light");
	menuNum=debugMenu.lastLayout();
	debugMenu.basicTextBox(&nameID, "Light Name: ", "Light Information", menuNum);
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.numberSpinBox(&position.x, "X: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&position.y, "Y: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&position.z, "Z: ", "Light Information", menuNum);
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.numberSpinBox(&color.x, "R: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&color.y, "G: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&color.z, "B: ", "Light Information", menuNum);
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.numberSpinBox(&size, "Size: ", "Light Information", menuNum);
	debugMenu.newRow("Light Information", menuNum);
	lightDelete=new QPushButton("Delete Light");
	debugMenu.pushButton(lightDelete, "Delete Light", "Light Information", menuNum);
	connect(lightDelete, SIGNAL(clicked()), this, SLOT(ligtDelete()));
	debugMenu.newRow("Light Information", menuNum);
	lightClone=new QPushButton("Clone Light");
	debugMenu.pushButton(lightClone, "Clone Light", "Light Information", menuNum);
	connect(lightClone, SIGNAL(clicked()), this, SLOT(ligtClone()));
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.manualChangeObjectSelected(menuNum);
#endif
}

DiffuseLight::~DiffuseLight()
{
	for(int i=0; i<components.size(); i++)
	{
		delete components[i];
	}
	components.clear();

	debugMenu.removeLayout(menuNum);
}

void DiffuseLight::ligtClone()
{
	cloneLight=true;
}

void DiffuseLight::ligtDelete()
{
	deleteLight=true;
}

void DiffuseLight::toggleMenu()
{
	debugMenu.toggleVisibility(menuNum);
}
void DiffuseLight::LightUpdate()
{
	//for(int i=0; i<components.size(); i++)
	//{
	//	components[i]->update();
	//}
}
void DiffuseLight::addComponent(Component* comp)
{
	components.push_back(comp);
	components[components.size()-1]->InitializeDLight(this, menuNum);
}

void DiffuseLight::setXColor(float x)
{
	color.x=x;
}
void DiffuseLight::setYColor(float y)
{
	color.y=y;
}
void DiffuseLight::setZColor(float z)
{
	color.z=z;
}