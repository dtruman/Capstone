#include "AmbientLight.h"
#include "Component.h"

AmbientLight::AmbientLight(std::string ID, glm::vec3 col, float inten)
{
	QWidget* tempLay=new QWidget();

	nameID=ID;
	tempLay->setFixedWidth(300);
	tempLay->setMinimumHeight(600);
	color=col;
	intensity=inten;
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
	debugMenu.numberSpinBox(&color.x, "Color Red: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&color.y, "Color Green: ", "Light Information", menuNum);
	debugMenu.numberSpinBox(&color.z, "Color Blue: ", "Light Information", menuNum);
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.slideFloat(&intensity, "Intensity", "Light Information", menuNum);
	debugMenu.newRow("Light Information");
	lightDelete=new QPushButton("Delete Light");
	debugMenu.pushButton(lightDelete, "Delete Light", "Light Information", menuNum);
	connect(lightDelete, SIGNAL(clicked()), this, SLOT(ligDelete()));
	debugMenu.newRow("Light Information", menuNum);
	lightClone=new QPushButton("Clone Light");
	debugMenu.pushButton(lightDelete, "Clone Light", "Light Information", menuNum);
	connect(lightClone, SIGNAL(clicked()), this, SLOT(ligClone()));
	debugMenu.newRow("Light Information", menuNum);
	debugMenu.manualChangeObjectSelected(menuNum);
#endif
}

void AmbientLight::ligDelete()
{
	deleteLight=true;
}

void AmbientLight::ligClone()
{
	cloneLight=true;
}

AmbientLight::~AmbientLight()
{
	for(int i=0; i<components.size(); i++)
	{
		delete components[i];
	}
	components.clear();

#if EDITOR_ON
	debugMenu.removeLayout(menuNum);
#endif
}

void AmbientLight::toggleMenu()
{
#if EDITOR_ON
	debugMenu.toggleVisibility(menuNum);
#endif
}
void AmbientLight::LightUpdate()
{
	//for(int i=0; i<components.size(); i++)
	//{
	//	components[i]->update();
	//}
}
void AmbientLight::addComponent(Component* comp)
{
	components.push_back(comp);
	components[components.size()-1]->InitializeLight(this, menuNum);
}

void AmbientLight::setXColor(float x)
{
	color.x=x;
}
void AmbientLight::setYColor(float y)
{
	color.y=y;
}
void AmbientLight::setZColor(float z)
{
	color.z=z;
}